#pragma once

#ifndef _CTM_CTM_ACCEPTOR_WEBSOCKET_ACCEPTOR_H_
#define _CTM_CTM_ACCEPTOR_WEBSOCKET_ACCEPTOR_H_

#include "./acceptor.hpp"

#include <Poco/Net/HTTPServer.h>

#include <memory>

namespace ctm::acceptor {
/**
 * @brief 웹 소켓 클라이언트 acceptor
 *
 */
class WebsocketAcceptor : public Acceptor {
public:
  /**
   * @brief Construct a new Websocket Acceptor object
   *
   */
  WebsocketAcceptor();
  /**
   * @brief Destroy the Websocket Acceptor object
   *
   */
  virtual ~WebsocketAcceptor();

  /**
   * @brief 클라이언트 접속 허용
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
  std::unique_ptr<Poco::Net::HTTPServer> websocket_server;
};
} // namespace ctm::acceptor

#endif