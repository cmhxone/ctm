#pragma once

#ifndef _CTM_CTM_HANDLER_TCP_HANDLER_HPP_
#define _CTM_CTM_HANDLER_TCP_HANDLER_HPP_

#include "../../channel/event/bridge_event.hpp"
#include "../../channel/event/client_event.hpp"
#include "../../channel/event_channel.hpp"
#include "../../channel/subscriber.hpp"
#include "../agent_info.hpp"
#include "../agent_info_map.hpp"

#include <asio/awaitable.hpp>
#include <asio/basic_stream_socket.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/ssl/stream.hpp>
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
      : client_socket(
            std::make_shared<asio::basic_stream_socket<asio::ip::tcp>>(
                std::move(client_socket))) {
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->subscribe(this);
  }
  /**
   * @brief Construct a new TCPHandler object
   *
   * @param ssl_socket
   */
  TCPHandler(
      std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> ssl_socket)
      : ssl_socket(std::move(ssl_socket)), client_socket(nullptr),
        ssl_enabled(true) {
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->subscribe(this);
  }

  /**
   * @brief Destroy the Asio Handler object
   *
   */
  virtual ~TCPHandler() {
    setRunning(true);
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->unsubscribe(this);
    try {
      if (ssl_enabled) {
        ssl_socket->next_layer().close();
      } else {
        client_socket->close();
      }
    } catch (...) {
    }
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
      if (ssl_enabled) {
        ssl_socket->write_some(
            asio::buffer(bride_event->getBridgeEventMessage().message));
      } else {
        client_socket->send(
            asio::buffer(bride_event->getBridgeEventMessage().message));
      }
    } catch (...) {
      // 전송 실패 시 끊어진 것으로 간주
      setRunning(false);
    }
  }

  /**
   * @brief 클라이언트 연결을 핸들링 한다
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> handleConnection() {
    setRunning(true);

    // 최초 접속 시, 전체 상담원 상태를 바이너리 메시지로 전송
    for (const std::pair<std::string, AgentInfo> &element :
         AgentInfoMap::getInstance()->get()) {
      if (ssl_enabled) {
        co_await ssl_socket->async_write_some(
            asio::buffer(element.second.pack()));
      } else {
        co_await client_socket->async_send(asio::buffer(element.second.pack()));
      }
    }

    while (isRunning()) {
      co_await read();
    }

    delete this;
  }

  /**
   * @brief TCP 소켓 메시지를 읽어들인다
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> read() {
    std::vector<std::byte> buffer{1'024};

    std::size_t length = 0;
    try {
      if (ssl_enabled) {
        length = co_await ssl_socket->async_read_some(asio::buffer(buffer),
                                                      asio::use_awaitable);
      } else {
        length = co_await client_socket->async_read_some(asio::buffer(buffer),
                                                         asio::use_awaitable);
      }
    } catch (...) {
      setRunning(false);
    }

    channel::EventChannel<channel::event::ClientEvent>::getInstance()->publish(
        channel::event::ClientEvent{buffer});
  }

  /**
   * @brief 실행 여부 반환
   *
   * @return true
   * @return false
   */
  constexpr bool isRunning() const {
    return is_running.load(std::memory_order_acquire);
  }

  /**
   * @brief Set the Running object
   *
   * @param is_running
   */
  void setRunning(const bool is_running) {
    this->is_running.store(is_running, std::memory_order_release);
  }

protected:
private:
  std::shared_ptr<asio::ssl::stream<asio::basic_stream_socket<asio::ip::tcp>>>
      ssl_socket;
  std::shared_ptr<asio::basic_stream_socket<asio::ip::tcp>> client_socket;
  bool ssl_enabled{false};
  std::atomic_bool is_running{false};
};
} // namespace ctm::handler

#endif