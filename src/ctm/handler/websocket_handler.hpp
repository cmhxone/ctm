#pragma once

#ifndef _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_HPP_
#define _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_HPP_

#define WEBSOCKET_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

#include "../../channel/event/bridge_event.hpp"
#include "../../channel/event/event.hpp"
#include "../../channel/event_channel.hpp"
#include "../../channel/subscriber.hpp"
#include "../../util/ini_loader.h"
#include "../agent_info.hpp"
#include "../agent_info_map.hpp"
#include "../message/state_request_message.hpp"

#include <Poco/Base64Encoder.h>
#include <Poco/SHA1Engine.h>
#include <asio/awaitable.hpp>
#include <asio/basic_stream_socket.hpp>
#include <asio/buffer.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/ssl/context.hpp>
#include <asio/ssl/stream.hpp>
#include <asio/use_awaitable.hpp>
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
protected:
  /**
   * @brief 웹 소켓 Fin bit
   *
   */
  enum WebsocketFin {
    FIN_TRUE = 0x80,
    FIN_FALSE = 0x00,
  };

  /**
   * @brief 웹 소켓 OpCode
   *
   */
  enum WebsocketOpCodes {
    CONTINUATION_FRAME = 0x00,
    TEXT_FRAME = 0x01,
    BINARY_FRAME = 0x02,
    CLOSE_FRAME = 0x08,
    PING_FRAME = 0x09,
    PONG_FRAME = 0x0A,
  };

public:
  /**
   * @brief Construct a new Websocket Handler object
   *
   * @param client_socket
   */
  WebsocketHandler(asio::ip::tcp::socket client_socket)
      : client_socket(
            std::make_shared<asio::basic_stream_socket<asio::ip::tcp>>(
                std::move(client_socket))) {
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->subscribe(this);
  }

  WebsocketHandler(
      std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> ssl_socket)
      : ssl_socket(std::move(ssl_socket)), client_socket(nullptr),
        ssl_enabled(true) {
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->subscribe(this);
  }

  //   WebsocketHandler(asio::) {}

  /**
   * @brief Destroy the Websocket Handler object
   *
   */
  virtual ~WebsocketHandler() {
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        ->unsubscribe(this);

    try {
      if (ssl_enabled) {
        ssl_socket->next_layer().shutdown(asio::socket_base::shutdown_both);
        ssl_socket->next_layer().close();
      } else {
        client_socket->shutdown(asio::socket_base::shutdown_both);
        client_socket->close();
      }
    } catch (...) {
    }
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

    try {
      sendBinary(bridge_event->getBridgeEventMessage().message);
    } catch (...) {
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

    while (isRunning()) {
      co_await read();
    }

    delete this;
  }

  /**
   * @brief 핸들러 실행 여부를 반환
   *
   * @return true
   * @return false
   */
  constexpr bool isRunning() const {
    return is_running.load(std::memory_order_acquire);
  }

protected:
  /**
   * @brief 패킷을 읽음
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> read() {
    std::vector<std::byte> buffer{4'096};

    std::size_t length = 0;
    try {
      length = ssl_enabled ? co_await ssl_socket->async_read_some(
                                 asio::buffer(buffer), asio::use_awaitable)
                           : co_await client_socket->async_read_some(
                                 asio::buffer(buffer), asio::use_awaitable);
    } catch (...) {
      setRunning(false);
      co_return;
    }

    std::vector<std::byte> packet{buffer.cbegin(), buffer.cbegin() + length};

    if (!isSwitched()) {
      spdlog::debug("Websocket upgrade requested");
      co_await sendSwitchingProtocol(packet);
      co_return;
    }

    spdlog::debug("Websocket read");
    co_await readWebsocketMessage(packet);
  }

  /**
   * @brief 웹소켓 프로토콜 스위칭 응답 전송
   *
   * @param data
   * @return asio::awaitable<void>
   */
  asio::awaitable<void>
  sendSwitchingProtocol(const std::vector<std::byte> &data) {

    if (!pathValidation(data)) {
      if (ssl_enabled) {
        co_await ssl_socket->async_write_some(
            asio::buffer(std::string("HTTP/1.1 400 Bad Request\r\n\r\n")));
        ssl_socket->next_layer().close();
      } else {
        co_await client_socket->async_send(
            asio::buffer(std::string("HTTP/1.1 400 Bad Request\r\n\r\n")));
        client_socket->close();
      }

      co_return;
    }

    std::unordered_map<std::string, std::string> http_header =
        std::move(parseHeader(data));

    // Upgrade, Sec-WebSocket-* 이 없을땐 400 오류 반환
    if (std::find_if(http_header.cbegin(), http_header.cend(),
                     [](const std::pair<std::string, std::string> &element) {
                       std::regex regexp{R"regex(Upgrade|Sec-WebSocket)regex"};
                       return !std::regex_search(element.first, regexp);
                     }) == http_header.cend()) {

      if (ssl_enabled) {
        co_await ssl_socket->async_write_some(
            asio::buffer(std::string("HTTP/1.1 400 Bad Request\r\n\r\n")));
        ssl_socket->next_layer().close();
      } else {
        co_await client_socket->async_send(
            asio::buffer(std::string("HTTP/1.1 400 Bad Request\r\n\r\n")));
        client_socket->close();
      }
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

    ssl_enabled ? co_await ssl_socket->async_write_some(
                      asio::buffer(response_stream.str()))
                : co_await client_socket->async_send(
                      asio::buffer(response_stream.str()));

    setSwitched(true);

    // 최초 접속 시, 전체 상담원 상태를 바이너리 메시지로 전송
    for (const std::pair<std::string, AgentInfo> &element :
         AgentInfoMap::getInstance()->get()) {
      sendBinary(element.second.pack());
    }
  }

  /**
   * @brief 웹소켓 URL 경로를 검증
   *
   * @param data
   * @return true
   * @return false
   */
  bool pathValidation(const std::vector<std::byte> &data) {
    std::ostringstream request_head{};

    for (int i = 0; i < data.size(); i++) {
      if (data.at(i) < static_cast<std::byte>(0x20)) {
        break;
      }
      request_head << static_cast<char>(data.at(i));
    }

    std::string line{request_head.str()};
    std::regex regexp{
        R"regex(^[A-Z]*\s([a-zA-Z0-9\/\-+_]*)\s[A-Z0-9\/.]*$)regex"};
    std::smatch match;

    if (!std::regex_match(line, match, regexp)) {
      return false;
    }

    return match[1].str() ==
           util::IniLoader::getInstance()->get("server", "websocket.path",
                                               std::string("/ctmonitor"));
  }

  /**
   * @brief HTTP 요청 헤더를 파싱
   *
   * @param data
   * @return std::unordered_map<std::string, std::string>
   */
  std::unordered_map<std::string, std::string>
  parseHeader(const std::vector<std::byte> &data) {
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
    std::ostringstream key_stream{};
    key_stream << key.data();
    key_stream << WEBSOCKET_GUID;

    // key + GUID -> SHA1 hash
    Poco::SHA1Engine sha1_engine{};
    sha1_engine.update(key_stream.str());

    // SHA1 hash -> Accept key
    std::ostringstream accept_stream{};
    Poco::Base64Encoder base64_encoder{accept_stream};
    const std::vector<unsigned char> digested = sha1_engine.digest();
    for (const unsigned char ch : digested) {
      base64_encoder.put(ch);
    }
    base64_encoder.close();

    return accept_stream.str();
  }

  /**
   * @brief 웹 소켓 메시지를 읽음
   *
   * @param message
   * @return asio::awaitable<void>
   */
  asio::awaitable<void>
  readWebsocketMessage(const std::vector<std::byte> &message) {
    std::byte frame_byte{0};
    frame_byte |= message.at(0);

    std::byte mask_byte{0};
    mask_byte |= message.at(1);

    std::size_t length = 0;
    std::size_t data_index = 2;

    // 전체 패킷 길이에 따라 데이터 시작 인덱스가 달라짐
    switch (mask_byte & static_cast<std::byte>(0x7F)) {
    case static_cast<std::byte>(126):
      for (int i = 0; i < 2; i++) {
        length |= static_cast<std::size_t>(message.at(data_index++));
        length <<= 8;
      }
      break;
    case static_cast<std::byte>(127):
      for (int i = 0; i < 8; i++) {
        length |= static_cast<std::size_t>(message.at(data_index++));
        length <<= 8;
      }
      break;
    default:
      length =
          static_cast<std::size_t>(mask_byte & static_cast<std::byte>(0x7F));
      break;
    }

    // 클라이언트 데이터는 항상 마스킹 되어있음 RFC 6455
    std::byte frame_mask[4] = {std::byte{0}, std::byte{0}, std::byte{0},
                               std::byte{0}};
    for (int i = 0; i < 4; i++) {
      frame_mask[i] = message.at(data_index++);
    }

    switch (frame_byte & static_cast<std::byte>(0x0F)) {
    case static_cast<std::byte>(WebsocketOpCodes::BINARY_FRAME): {
      // 이진 프레임 처리
      std::size_t payload_index = 0;
      std::vector<std::byte> buffer{};
      std::for_each(message.cbegin() + data_index, message.cend(),
                    [&](const std::byte &b) {
                      buffer.emplace_back(b ^ frame_mask[payload_index++ % 4]);
                    });

      // do something with buffer
    } break;
    case static_cast<std::byte>(WebsocketOpCodes::TEXT_FRAME): {
      // 문자열 프레임 처리
      std::ostringstream stream{};
      std::size_t payload_index = 0;
      std::for_each(message.cbegin() + data_index, message.cend(),
                    [&](const std::byte &b) {
                      stream << static_cast<char>(
                          b ^ frame_mask[payload_index++ % 4]);
                    });

      spdlog::debug("websocket received: {}", stream.str());

      if (stream.str().starts_with("query_agent:")) {
        message::StateRequestMessage state_request_message{};
        state_request_message.addAgent(stream.str().data() +
                                       std::string("query_agent:").length());

        // channel::EventChannel<channel::event::BridgeEvent>::getInstance()
        //     ->publish(channel::event::BridgeEvent{
        //         channel::event::BridgeEvent::BridgeEventDestination::CTI});
      }
    } break;
    default:
      break;
    }

    co_return;
  }

  /**
   * @brief 웹 소켓 텍스트 메시지를 전송
   *
   * @param message
   */
  void sendText(const std::string_view message) {
    std::vector<std::byte> buffer{};

    buffer.emplace_back(static_cast<std::byte>(WebsocketFin::FIN_TRUE) |
                        static_cast<std::byte>(WebsocketOpCodes::TEXT_FRAME));

    if (message.length() < 126) {
      buffer.emplace_back(static_cast<std::byte>(message.length()));
    } else if (message.length() < 65535) {
      buffer.emplace_back(static_cast<std::byte>(126));
      buffer.emplace_back(static_cast<std::byte>(message.length() & 0xFF00));
      buffer.emplace_back(static_cast<std::byte>(message.length() & 0x00FF));
    } else {
      // FIXME: 16,384 바이트 이상 전송 시, 네트워크가 프레임을 쪼개서 전송하기
      // 때문에 전체 바이트 전송이 되지 않는다. 이는 프레임을 쪼개서 전송하는
      // 방식으로 변경이 필요함. 현재는 그렇게 긴 데이터를 전송하는 경우가 없어
      // 이대로 유지하겠음
      buffer.emplace_back(static_cast<std::byte>(127));
      buffer.emplace_back(
          static_cast<std::byte>(message.length() & 0xFF00'0000'0000'0000));
      buffer.emplace_back(
          static_cast<std::byte>(message.length() & 0x00FF'0000'0000'0000));
      buffer.emplace_back(
          static_cast<std::byte>(message.length() & 0x0000'FF00'0000'0000));
      buffer.emplace_back(
          static_cast<std::byte>(message.length() & 0x0000'00FF'0000'0000));
      buffer.emplace_back(
          static_cast<std::byte>(message.length() & 0x0000'0000'FF00'0000));
      buffer.emplace_back(
          static_cast<std::byte>(message.length() & 0x0000'0000'00FF'0000));
      buffer.emplace_back(
          static_cast<std::byte>(message.length() & 0x0000'0000'0000'FF00));
      buffer.emplace_back(
          static_cast<std::byte>(message.length() & 0x0000'0000'0000'00FF));
    }

    std::for_each(message.cbegin(), message.cend(), [&](const char ch) {
      buffer.emplace_back(static_cast<std::byte>(ch));
    });

    ssl_enabled ? ssl_socket->write_some(asio::buffer(buffer))
                : client_socket->send(asio::buffer(buffer));
    return;
  }

  /**
   * @brief 웹 소켓 바이너리 메시지를 전송
   *
   * @param data
   */
  void sendBinary(const std::vector<std::byte> &data) {
    std::vector<std::byte> buffer{};

    buffer.emplace_back(static_cast<std::byte>(WebsocketFin::FIN_TRUE) |
                        static_cast<std::byte>(WebsocketOpCodes::BINARY_FRAME));

    if (data.size() < 126) {
      buffer.emplace_back(static_cast<std::byte>(data.size()));
    } else if (data.size() < 65535) {
      buffer.emplace_back(static_cast<std::byte>(126));
      buffer.emplace_back(static_cast<std::byte>(data.size() & 0xFF00));
      buffer.emplace_back(static_cast<std::byte>(data.size() & 0x00FF));
    } else {
      // FIXME: 16,384 바이트 이상 전송 시, 네트워크가 프레임을 쪼개서 전송하기
      // 때문에 전체 바이트 전송이 되지 않는다. 이는 프레임을 쪼개서 전송하는
      // 방식으로 변경이 필요함. 현재는 그렇게 긴 데이터를 전송하는 경우가 없어
      // 이대로 유지하겠음
      buffer.emplace_back(static_cast<std::byte>(127));
      buffer.emplace_back(
          static_cast<std::byte>(data.size() & 0xFF00'0000'0000'0000));
      buffer.emplace_back(
          static_cast<std::byte>(data.size() & 0x00FF'0000'0000'0000));
      buffer.emplace_back(
          static_cast<std::byte>(data.size() & 0x0000'FF00'0000'0000));
      buffer.emplace_back(
          static_cast<std::byte>(data.size() & 0x0000'00FF'0000'0000));
      buffer.emplace_back(
          static_cast<std::byte>(data.size() & 0x0000'0000'FF00'0000));
      buffer.emplace_back(
          static_cast<std::byte>(data.size() & 0x0000'0000'00FF'0000));
      buffer.emplace_back(
          static_cast<std::byte>(data.size() & 0x0000'0000'0000'FF00));
      buffer.emplace_back(
          static_cast<std::byte>(data.size() & 0x0000'0000'0000'00FF));
    }

    std::move(data.cbegin(), data.cend(), std::back_inserter(buffer));
    ssl_enabled ? ssl_socket->write_some(asio::buffer(buffer))
                : client_socket->send(asio::buffer(buffer));
    return;
  }

  /**
   * @brief 웹 소켓 퐁 메시지를 전송
   *
   */
  void sendPong() {
    std::vector<std::byte> buffer{};

    buffer.emplace_back(static_cast<std::byte>(WebsocketFin::FIN_TRUE) |
                        static_cast<std::byte>(WebsocketOpCodes::PONG_FRAME));
    buffer.emplace_back(static_cast<std::byte>(0));

    ssl_enabled ? ssl_socket->write_some(asio::buffer(buffer))
                : client_socket->send(asio::buffer(buffer));
    return;
  }

  /**
   * @brief 웹 소켓 종료 메시지를 전송
   *
   */
  void sendClose() {
    std::vector<std::byte> buffer{};

    buffer.emplace_back(static_cast<std::byte>(WebsocketFin::FIN_TRUE) |
                        static_cast<std::byte>(WebsocketOpCodes::CLOSE_FRAME));
    buffer.emplace_back(static_cast<std::byte>(0));

    if (ssl_enabled) {
      ssl_socket->write_some(asio::buffer(buffer));
      ssl_socket->next_layer().close();
    } else {
      client_socket->send(asio::buffer(buffer));
      client_socket->close();
    }
    return;
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
  std::shared_ptr<asio::ssl::stream<asio::basic_stream_socket<asio::ip::tcp>>>
      ssl_socket;
  std::shared_ptr<asio::basic_stream_socket<asio::ip::tcp>> client_socket;
  bool ssl_enabled{false};
  std::atomic_bool is_running{false};
  std::atomic_bool is_switched{false};
};
} // namespace ctm::handler

#endif
