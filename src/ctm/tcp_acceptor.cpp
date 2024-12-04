#include "./tcp_acceptor.h"
#include "../util/ini_loader.h"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAcceptor.h>
#include <Poco/Thread.h>

#include <spdlog/spdlog.h>

using namespace std;
using namespace channel::event;

namespace ctm {
/**
 * @brief Construct a new TCPAcceptor::TCPAcceptor object
 *
 */
TCPAcceptor::TCPAcceptor()
    : server_socket(
          util::IniLoader::getInstance()->get("server", "tcp.port", 5110)),
      server_socket_reactor(),
      server_socket_acceptor(server_socket, server_socket_reactor) {

  server_socket.setLinger(true, 3);
  server_socket.setNoDelay(false);
  server_socket.setReuseAddress(true);

  spdlog::debug(
      "TCP Acceptor constructed. port: {}",
      util::IniLoader::getInstance()->get("server", "tcp.port", 5110));
}

/**
 * @brief Destroy the TCPAcceptor::TCPAcceptor object
 *
 */
TCPAcceptor::~TCPAcceptor() {
  server_socket_reactor.stop();
  reactor_thread.join();
}

/**
 * @brief 클라이언트 접속을 처리한다
 *
 */
void TCPAcceptor::accept() noexcept {
  reactor_thread.start(server_socket_reactor);
}

/**
 * @brief 이벤트 핸들러
 *
 * @param event
 */
void TCPAcceptor::handleEvent(const Event *event) {}
} // namespace ctm