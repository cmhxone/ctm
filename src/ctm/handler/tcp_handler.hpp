#pragma once

#ifndef _CTM_CTM_HANDLER_TCP_HANDLER_HPP_
#define _CTM_CTM_HANDLER_TCP_HANDLER_HPP_

#include "../../channel/event/bridge_event.hpp"
#include "../../channel/event/client_event.hpp"
#include "../../channel/event_channel.hpp"
#include "../../channel/subscriber.hpp"

#include <asio/awaitable.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/use_awaitable.hpp>
#include <spdlog/spdlog.h>

#include <atomic>
#include <cstddef>
#include <vector>

namespace ctm::handler {
/**
 * @brief TCP 서버 핸들러
 *
 */
class TCPHandler : public channel::Subscriber {
public:
  /**
   * @brief Construct a new Asio Handler object
   *
   * @param client_socket
   */
  TCPHandler(asio::ip::tcp::socket client_socket)
      : client_socket(std::move(client_socket)) {
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->subscribe(this);
  }
  /**
   * @brief Destroy the Asio Handler object
   *
   */
  virtual ~TCPHandler() {
    is_running.store(false, std::memory_order_release);
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->unsubscribe(this);
  };

  /**
   * @brief 이벤트 핸들러
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override {
    if (event->getEventType() != channel::event::EventType::BRIDGE_EVENT) {
      return;
    }

    const channel::event::BridgeEvent *bride_event =
        dynamic_cast<const channel::event::BridgeEvent *>(event);

    if (bride_event->getDestination() !=
        channel::event::BridgeEvent::BridgeEventDestination::CLIENT) {
      return;
    }

    // 이벤트 수신 시, 클라이언트로 메시지를 전송한다
    try {
      client_socket.send(
          asio::buffer(bride_event->getBridgeEventMessage().message));
    } catch (...) {
      // 전송 실패 시 끊어진 것으로 간주
      is_running.store(false, std::memory_order_release);
    }
  }

  /**
   * @brief 클라이언트 연결을 핸들링 한다
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> handleConnection() {
    is_running.store(true, std::memory_order_release);

    while (is_running.load(std::memory_order_acquire)) {
      co_await read();
    }
  }

  /**
   * @brief TCP 소켓 메시지를 읽어들인다
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> read() {
    std::vector<std::byte> buffer{1'024};

    std::size_t length = co_await client_socket.async_read_some(
        asio::buffer(buffer), asio::use_awaitable);

    spdlog::debug("Client sent. peer_address: {}, length: {}",
                  client_socket.remote_endpoint().address().to_v4().to_string(),
                  length);

    channel::EventChannel<channel::event::ClientEvent>::getInstance()->publish(
        channel::event::ClientEvent{buffer});
  }

protected:
private:
  asio::ip::tcp::socket client_socket;
  std::atomic_bool is_running{false};
};
} // namespace ctm::handler

#endif