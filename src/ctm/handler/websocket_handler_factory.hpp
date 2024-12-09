#pragma once

#ifndef _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_FACTORY_HPP_
#define _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_FACTORY_HPP_

#include "./websocket_handler.h"

#include "../../util/ini_loader.h"

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <spdlog/spdlog.h>

#include <string>

namespace ctm::handler {
/**
 * @brief 웹 소켓 핸들러 팩토리
 *
 */
class WebsocketHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
  /**
   * @brief Construct a new Websocket Handler Factory object
   *
   */
  WebsocketHandlerFactory() {}
  /**
   * @brief Destroy the Websocket Handler Factory object
   *
   */
  virtual ~WebsocketHandlerFactory() = default;

  /**
   * @brief Create a Request Handler object
   *
   * @param request
   * @return Poco::Net::HTTPRequestHandler*
   */
  virtual Poco::Net::HTTPRequestHandler *
  createRequestHandler(const Poco::Net::HTTPServerRequest &request) override {
    if (request.getURI() !=
        util::IniLoader::getInstance()->get("server", "websocket.path",
                                            std::string("/ctmonitor"))) {
      return nullptr;
    }

    spdlog::debug("Websocket client accepted. peer_addr: {}",
                  request.clientAddress().toString());

    return new WebsocketHandler();
  }

protected:
private:
};
} // namespace ctm::handler

#endif