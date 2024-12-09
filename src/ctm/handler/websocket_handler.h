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
/**
 * @brief 웹 소켓 핸들러
 *
 */
class WebsocketHandler : public Poco::Net::HTTPRequestHandler,
                         public channel::Subscriber {
public:
  /**
   * @brief Construct a new Websocket Handler object
   *
   */
  WebsocketHandler();
  /**
   * @brief Destroy the Websocket Handler object
   *
   */
  virtual ~WebsocketHandler();

  /**
   * @brief 웹 소켓 핸들러
   *
   * @param request
   * @param response
   */
  virtual void handleRequest(Poco::Net::HTTPServerRequest &request,
                             Poco::Net::HTTPServerResponse &response) override;

  /**
   * @brief 이벤트 핸들러
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override;

protected:
private:
  std::unique_ptr<Poco::Net::WebSocket> websocket;
  std::vector<std::byte> receive_buffer{2'048};
};
} // namespace ctm::handler

#endif