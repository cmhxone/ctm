#include "./ctm.h"

#include "../channel/event/cti_error_event.hpp"
#include "../channel/event/error_event.hpp"
#include "../channel/event_channel.hpp"
#include "../util/ini_loader.h"
#include "./acceptor/acceptor.hpp"
#include "./acceptor/tcp_acceptor.hpp"
#include "./acceptor/websocket_acceptor.hpp"
#include "./bridge/message_bridge.hpp"
#include "./client_state.hpp"
#include "./cti_client.h"

#include <chrono>
#include <memory>
#include <spdlog/spdlog.h>
#include <thread>

using namespace std;
using namespace channel::event;

namespace ctm {
/**
 * @brief Construct a new CTM::CTM object
 *
 */
CTM::CTM() {
  channel::EventChannel<channel::event::CTIErrorEvent>::getInstance()
      ->subscribe(this);

  bridge::MessageBridge::getInstance();

  // CTI Client 생성 및 접속
  cti_client = make_unique<CTIClient>();
  cti_client->connect();

  if (util::IniLoader::getInstance()->get("server", "tcp.enabled", false)) {
    acceptors.emplace_back(make_unique<acceptor::TCPAcceptor>());
  }

  if (util::IniLoader::getInstance()->get("server", "websocket.enabled",
                                          false)) {
    acceptors.emplace_back(make_unique<acceptor::WebsocketAcceptor>());
  }

  for (unique_ptr<acceptor::Acceptor> &acceptor : acceptors) {
    acceptor->accept();
  }
}

/**
 * @brief Destroy the CTM::CTM object
 *
 */
CTM::~CTM() {
  channel::EventChannel<channel::event::CTIErrorEvent>::getInstance()
      ->unsubscribe(this);
}

/**
 * @brief 이벤트 핸들러
 *
 * @param event
 */
void CTM::handleEvent(const Event *event) {
  switch (event->getEventType()) {
  case EventType::CTI_EVENT:
    // CTI 이벤트
    break;
  case ::EventType::ERROR_EVENT:
    // 오류 이벤트
    switch (dynamic_cast<const ErrorEvent *>(event)->getErrorType()) {
    case ErrorType::CTI_ERROR:
      // CTI 오류
      dynamic_cast<const CTIErrorEvent *>(event)->getCTIErrorType();
      dynamic_cast<const CTIErrorEvent *>(event)->getErrorHost();

      spdlog::warn(
          "CTI Error notified. error_host: {}, error_type: {}",
          dynamic_cast<const CTIErrorEvent *>(event)->getErrorHost(),
          static_cast<std::uint32_t>(
              dynamic_cast<const CTIErrorEvent *>(event)->getCTIErrorType()));

      // 이중화 절체
      ClientState::getInstance()->toggleActive();

      // CG는 곧바로 SideB로 절체되지 않는다
      // 내부적인 동기화 작업에 일정 시간이 소요됨으로 인해
      // 스레드 sleep 코드를 추가하였다
      // 안전하지 않음. 유의바람.
      this_thread::sleep_for(chrono::milliseconds{500});

      // CTI Client 재생성
      cti_client = make_unique<CTIClient>();
      cti_client->connect();
      break;
    case ErrorType::INTERNAL_ERROR:
      // 내부 오류
      break;
    case ErrorType::CLIENT_ERROR:
      // 클라이언트 오류
      break;
    }
    break;
  default:
    break;
  }
}
} // namespace ctm