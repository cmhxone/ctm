#pragma once

#ifndef _CTM_CTM_TCP_ACCEPTOR_H_
#define _CTM_CTM_TCP_ACCEPTOR_H_

#include "./acceptor.hpp"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAcceptor.h>
#include <Poco/Net/SocketReactor.h>

namespace ctm {
class TCPAcceptor : public Acceptor {
public:
  /**
   * @brief Construct a new TCPAcceptor object
   *
   */
  TCPAcceptor();

  /**
   * @brief Destroy the TCPAcceptor object
   *
   */
  virtual ~TCPAcceptor() = default;

  /**
   * @brief 클라이언트 접속을 처리한다
   *
   */
  virtual void accept() noexcept override;

protected:
private:
  Poco::Net::ServerSocket server_socket;
  Poco::Net::SocketReactor server_socket_reactor;
  Poco::Net::SocketAcceptor<TCPAcceptor> server_socket_acceptor;
};
} // namespace ctm

#endif