#pragma once

#ifndef _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_H_
#define _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_H_

#include "../../channel/subscriber.hpp"

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/WebSocket.h>

#include <cstddef>
#include <memory>
#include <vector>

namespace ctm::handler {
class WebsocketHandler : public Poco::Net::HTTPRequestHandler,
                         public channel::Subscriber {
public:
  WebsocketHandler();
  virtual ~WebsocketHandler();

  virtual void handleRequest(Poco::Net::HTTPServerRequest &request,
                             Poco::Net::HTTPServerResponse &response) override;

  virtual void handleEvent(const channel::event::Event *event) override;

protected:
private:
  std::unique_ptr<Poco::Net::WebSocket> websocket;
  std::vector<std::byte> receive_buffer{2'048};
};
} // namespace ctm::handler

#endif