#pragma once

#ifndef _CTM_CTM_ACCEPTOR_TCP_ACCEPTOR_H_
#define _CTM_CTM_ACCEPTOR_TCP_ACCEPTOR_H_

#include "./acceptor.hpp"
#include "../handler/tcp_handler.h"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAcceptor.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Thread.h>

namespace ctm::acceptor {
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
  virtual ~TCPAcceptor();

  /**
   * @brief 클라이언트 접속을 처리한다
   *
   */
  virtual void accept() noexcept override;

  /**
   * @brief 이벤트 핸들러
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override;

protected:
private:
  Poco::Net::ServerSocket server_socket;
  Poco::Net::SocketReactor server_socket_reactor;
  Poco::Net::SocketAcceptor<handler::TCPHandler> server_socket_acceptor;
  Poco::Thread reactor_thread;
};
} // namespace ctm

#endif