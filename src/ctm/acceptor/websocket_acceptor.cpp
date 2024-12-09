#include "./websocket_acceptor.h"

#include "../../util/ini_loader.h"
#include "../handler/websocket_handler_factory.hpp"

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <thread>

using namespace std;
using namespace channel::event;
using namespace Poco::Net;

namespace ctm::acceptor {
/**
 * @brief Construct a new Websocket Acceptor:: Websocket Acceptor object
 *
 */
WebsocketAcceptor::WebsocketAcceptor() {
  HTTPServerParams *params = new HTTPServerParams();
  params->setMaxQueued(64 * thread::hardware_concurrency());
  params->setMaxThreads(thread::hardware_concurrency());

  websocket_server = make_unique<HTTPServer>(
      new handler::WebsocketHandlerFactory(),
      util::IniLoader::getInstance()->get("server", "websocket.port", 8085),
      params);
}

/**
 * @brief Destroy the Websocket Acceptor:: Websocket Acceptor object
 *
 */
WebsocketAcceptor::~WebsocketAcceptor() { websocket_server->stop(); }

/**
 * @brief 웹 소켓 클라이언트 접속 허용
 *
 */
void WebsocketAcceptor::accept() noexcept { websocket_server->start(); }

/**
 * @brief 이벤트 핸들러
 *
 * @param event
 */
void WebsocketAcceptor::handleEvent(const Event *event) {}

} // namespace ctm::acceptor