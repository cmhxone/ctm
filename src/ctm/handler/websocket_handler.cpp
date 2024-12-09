#include "./websocket_handler.h"

#include "../../channel/event/bridge_event.hpp"
#include "../../channel/event_channel.hpp"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/WebSocket.h>
#include <spdlog/spdlog.h>

using namespace std;
using namespace Poco::Net;
using namespace channel;
using namespace channel::event;

namespace ctm::handler {
/**
 * @brief Construct a new Websocket Handler:: Websocket Handler object
 *
 */
WebsocketHandler::WebsocketHandler() {
  EventChannel<BridgeEvent>::getInstance()->subscribe(this);
}

/**
 * @brief Destroy the Websocket Handler:: Websocket Handler object
 *
 */
WebsocketHandler::~WebsocketHandler() {
  EventChannel<BridgeEvent>::getInstance()->unsubscribe(this);
}

/**
 * @brief 웹 소켓 핸들러
 *
 * @param request
 * @param response
 */
void WebsocketHandler::handleRequest(HTTPServerRequest &request,
                                     HTTPServerResponse &response) {
  websocket = make_unique<WebSocket>(request, response);

  int frame_flag = WebSocket::FRAME_BINARY | WebSocket::FRAME_TEXT;
  while (size_t length =
             websocket->receiveFrame(receive_buffer.data(),
                                     receive_buffer.size(), frame_flag) > 0) {
    spdlog::debug("Frame received. length: {}", length);
  }
}

/**
 * @brief 이벤트 핸들러
 *
 * @param event
 */
void WebsocketHandler::handleEvent(const Event *event) {}
} // namespace ctm::handler