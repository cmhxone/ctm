#pragma once

#ifndef _CTM_CTM_ACCEPTOR_WEBSOCKET_ACCEPTOR_H_
#define _CTM_CTM_ACCEPTOR_WEBSOCKET_ACCEPTOR_H_

#include "./acceptor.hpp"

#include <Poco/Net/HTTPServer.h>

#include <memory>

namespace ctm::acceptor {
class WebsocketAcceptor : public Acceptor {
public:
  WebsocketAcceptor();
  virtual ~WebsocketAcceptor();

  virtual void accept() noexcept override;

  virtual void handleEvent(const channel::event::Event *event) override;

protected:
private:
  std::unique_ptr<Poco::Net::HTTPServer> websocket_server;
};
} // namespace ctm::acceptor

#endif