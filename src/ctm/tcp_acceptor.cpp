#include "./tcp_acceptor.h"
#include "../util/ini_loader.h"

#include <Poco/Net/ServerSocket.h>

#include <Poco/Net/SocketAcceptor.h>
#include <spdlog/spdlog.h>

using namespace std;

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

  spdlog::debug(
      "TCP Acceptor constructed. port: {}",
      util::IniLoader::getInstance()->get("server", "tcp.port", 5110));
}

/**
 * @brief 클라이언트 접속을 처리한다
 *
 */
void TCPAcceptor::accept() noexcept {}
} // namespace ctm