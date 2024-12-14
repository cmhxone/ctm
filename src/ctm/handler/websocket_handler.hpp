#pragma once

#ifndef _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_HPP_
#define _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_HPP_

#define WEBSOCKET_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

#include "../../channel/subscriber.hpp"
#include "../../channel/event/bridge_event.hpp"
#include "../../channel/event/event.hpp"
#include "../../channel/event_channel.hpp"

#include <asio/awaitable.hpp>
#include <asio/buffer.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/use_awaitable.hpp>
#include <cryptopp/base64.h>
#include <cryptopp/sha.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ctm::handler {
class WebsocketHandler : public channel::Subscriber {
public:
  /**
   * @brief Construct a new Websocket Handler object
   *
   * @param client_socket
   */
  WebsocketHandler(asio::ip::tcp::socket client_socket)
      : client_socket(std::move(client_socket)) {
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->subscribe(this);
  }

  /**
   * @brief Destroy the Websocket Handler object
   *
   */
  virtual ~WebsocketHandler() {
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->unsubscribe(this);
  }

  /**
   * @brief 이벤트 핸들러
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override {
    if (event->getEventType() != channel::event::EventType::BRIDGE_EVENT) {
      return;
    }

    const channel::event::BridgeEvent *bridge_event =
        dynamic_cast<const channel::event::BridgeEvent *>(event);

    if (bridge_event->getDestination() !=
        channel::event::BridgeEvent::BridgeEventDestination::CLIENT) {
      return;
    }
  }

  /**
   * @brief 클라이언트 연결을 핸들링 한다
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> handleConnection() {
    setRunning(true);

    while (isRunning()) {
      co_await read();
    }
  }

protected:
  /**
   * @brief 패킷을 읽음
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> read() {
    std::vector<std::byte> buffer{4'096};

    std::size_t length = co_await client_socket.async_read_some(
        asio::buffer(buffer), asio::use_awaitable);

    if (!isSwitched()) {
      spdlog::debug("Websocket upgrade requested");
      co_await sendSwitchingProtocol(buffer, length);
      co_return;
    }

    spdlog::debug("Websocket read");
  }

  /**
   * @brief 웹소켓 프로토콜 스위칭 응답 전송
   *
   * @param data
   * @param length
   * @return asio::awaitable<void>
   */
  asio::awaitable<void>
  sendSwitchingProtocol(const std::vector<std::byte> &data,
                        std::size_t length) {
    std::unordered_map<std::string, std::string> http_header =
        std::move(parseHeader(data, length));

    // Upgrade, Sec-WebSocket-* 이 없을땐 400 오류 반환
    if (std::find_if(http_header.cbegin(), http_header.cend(),
                     [](const std::pair<std::string, std::string> &element) {
                       std::regex regexp{R"regex(Upgrade|Sec-WebSocket)regex"};
                       return !std::regex_search(element.first, regexp);
                     }) == http_header.cend()) {
      co_await client_socket.async_send(
          asio::buffer(std::string("HTTP/1.1 400 Bad Request\r\n\r\n")));
      client_socket.close();
      co_return;
    }

    std::stringstream response_stream{};
    response_stream << "HTTP/1.1 101 Switching Protocols\r\n"
                    << "Upgrade: " << http_header.at("Upgrade") << "\r\n"
                    << "Connection: Upgrade\r\n"
                    << "Sec-WebSocket-Accept: "
                    << getSecWebSocketAccept(
                           http_header.at("Sec-WebSocket-Key"))
                    << "\r\n\r\n";

    co_await client_socket.async_send(asio::buffer(response_stream.str()));
    setSwitched(true);
  }

  /**
   * @brief HTTP 요청 헤더를 파싱
   *
   * @param data
   * @param length
   * @return std::unordered_map<std::string, std::string>
   */
  std::unordered_map<std::string, std::string>
  parseHeader(const std::vector<std::byte> &data, std::size_t length) {
    std::vector<std::string> headers;

    std::unordered_map<std::string, std::string> result{};
    auto spos = data.cbegin();
    while (spos != data.cend()) {
      auto epos = std::find(spos, data.cend(), static_cast<std::byte>('\n'));

      std::ostringstream line_stream{};
      for (const std::byte b : std::vector<std::byte>{spos, epos}) {
        if (static_cast<char>(b) == '\r') {
          continue;
        }
        line_stream << static_cast<char>(b);
      }
      //   spdlog::debug("\t\t\t{}", line_stream.str());
      spos = epos != data.cend() ? epos + 1 : epos;

      const std::string line_string = line_stream.str();
      const std::regex regexp{
          R"regex(^([0-9a-zA-Z\-]*):\s([0-9a-zA-Z=/, :.+\-=*;()]*)$)regex"};
      std::smatch match;

      if (!std::regex_match(line_string, match, regexp)) {
        // spdlog::debug("\t\t\t'{}' is not a header message", line_string);
        continue;
      }

      result.insert(std::make_pair<std::string, std::string>(match[1].str(),
                                                             match[2].str()));
    }

    return result;
  }

  /**
   * @brief Get the Sec Web Socket Accept object
   *
   * @param key
   * @return const std::string
   */
  const std::string getSecWebSocketAccept(const std::string_view &key) {
    std::ostringstream accept_stream{};
    accept_stream << key.data();
    accept_stream << WEBSOCKET_GUID;

    // key + GUID -> SHA1 hash
    CryptoPP::SHA1 hasher;
    CryptoPP::byte sha1_hash[CryptoPP::SHA1::DIGESTSIZE];
    hasher.CalculateDigest(sha1_hash,
                           (const CryptoPP::byte *)accept_stream.str().data(),
                           accept_stream.str().size());

    // SHA1 hash -> Accept key
    CryptoPP::Base64Encoder encoder{nullptr, false};
    std::string result;
    encoder.Attach(new CryptoPP::StringSink(result));
    encoder.Put(sha1_hash, CryptoPP::SHA1::DIGESTSIZE);
    encoder.MessageEnd();

    return result;
  }

  /**
   * @brief 웹 소켓 텍스트 메시지를 전송
   *
   * @param message
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> sendText(const std::string_view message) {}

  /**
   * @brief 웹 소켓 바이너리 메시지를 전송
   *
   * @param data
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> sendBinary(const std::vector<std::byte> &data) {}

  /**
   * @brief 웹 소켓 퐁 메시지를 전송
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> sendPong() {}

  /**
   * @brief 웹 소켓 종료 메시지를 전송
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> sendClose() {}

  /**
   * @brief 핸들러 실행 여부를 반환
   *
   * @return true
   * @return false
   */
  constexpr bool isRunning() const {
    return is_running.load(std::memory_order_acquire);
  }

  /**
   * @brief 웹 소켓 프로토콜 전환 여부를 반환
   *
   * @return true
   * @return false
   */
  constexpr bool isSwitched() const {
    return is_switched.load(std::memory_order_acquire);
  }

  /**
   * @brief Set the Is Running object
   *
   * @param is_running
   */
  void setRunning(const bool is_running) {
    this->is_running.store(is_running, std::memory_order_release);
  }

  /**
   * @brief Set the Is Switched object
   *
   * @param is_switched
   */
  void setSwitched(const bool is_switched) {
    this->is_switched.store(is_switched, std::memory_order_release);
  }

private:
  asio::ip::tcp::socket client_socket;
  std::atomic_bool is_running{false};
  std::atomic_bool is_switched{false};
};
} // namespace ctm::handler

#endif
