#include "./tcp_handler.h"

#include "../../channel/event/client_event.hpp"
#include "../../channel/event_channel.hpp"

#include <Poco/AutoPtr.h>
#include <Poco/NObserver.h>
#include <Poco/Net/SocketNotification.h>

#include <spdlog/spdlog.h>

#include <cstdint>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace channel::event;

namespace ctm::handler {
/**
 * @brief Construct a new TCPHandler::TCPHandler object
 *
 * @param socket
 * @param reactor
 */
TCPHandler::TCPHandler(Poco::Net::StreamSocket &socket,
                       Poco::Net::SocketReactor &reactor)
    : Handler(socket, reactor) {
  spdlog::debug("TCPHandler constructed. client_address: {}",
                socket.peerAddress().toString());
}

/**
 * @brief 읽어들일 패킷이 있는 경우
 *
 */
void TCPHandler::onReadable(
    const Poco::AutoPtr<Poco::Net::ReadableNotification> &notification) {
  size_t length =
      socket.receiveBytes(receive_buffer.data(), receive_buffer.size());

  // 아무런 입력도 들어오지 않는 경우, 커넥션이 종료되었다고 판단 함
  if (length == 0) {
    socket.close();
    delete this;
    return;
  }

  stringstream ss{};
  for (int i = 0; i < length; i++) {
    ss << std::setw(2) << std::setfill('0') << std::hex
       << static_cast<int32_t>(receive_buffer.at(i)) << " ";

    if (i % 4 == 3) {
      ss << " ";
    }

    if (i % 16 == 15) {
      ss << "\n";
    }
  }

  // 클라이언트 이벤트 발생
  channel::EventChannel<ClientEvent>::getInstance()->publish(
      ClientEvent{receive_buffer});

  spdlog::debug("Client sent packet. client_address: {}\n{}",
                socket.peerAddress().toString(), ss.str());
}

/**
 * @brief 오류가 발생한 경우
 *
 */
void TCPHandler::onError(
    const Poco::AutoPtr<Poco::Net::ErrorNotification> &notification) {
  spdlog::debug("Client error. client_address: {}",
                socket.peerAddress().toString());
  delete this;
}

/**
 * @brief 커넥션이 종료된 경우
 *
 */
void TCPHandler::onShutdown(
    const Poco::AutoPtr<Poco::Net::ShutdownNotification> &notification) {
  spdlog::debug("Client disconnected. client_address: {}",
                socket.peerAddress().toString());

  delete this;
}

} // namespace ctm::handler