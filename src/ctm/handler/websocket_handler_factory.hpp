#pragma once

#ifndef _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_FACTORY_HPP_
#define _CTM_CTM_HANDLER_WEBSOCKET_HANDLER_FACTORY_HPP_

#include "./websocket_handler.h"

#include "../../util/ini_loader.h"

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include <string>

namespace ctm::handler {
class WebsocketHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
  WebsocketHandlerFactory() {}
  virtual ~WebsocketHandlerFactory() = default;

  virtual Poco::Net::HTTPRequestHandler *
  createRequestHandler(const Poco::Net::HTTPServerRequest &request) override {
    if (request.getURI() !=
        util::IniLoader::getInstance()->get("server", "websocket.path",
                                            std::string("/ctmonitor"))) {
      return nullptr;
    }

    return new WebsocketHandler();
  }

protected:
private:
};
} // namespace ctm::handler

#endif