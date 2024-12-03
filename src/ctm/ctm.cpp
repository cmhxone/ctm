#include "./ctm.h"

#include "../channel/event/cti_error_event.hpp"
#include "../channel/event/error_event.hpp"
#include "./client_state.hpp"
#include "cti_client.h"
#include <memory>

using namespace std;
using namespace channel::event;

namespace ctm {
/**
 * @brief Construct a new CTM::CTM object
 *
 */
CTM::CTM() {
  // CTI Client 생성 및 접속
  cti_client = make_unique<CTIClient>();
  cti_client->connect();
}

/**
 * @brief
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

      // 이중화 절체
      ClientState::getInstance()->toggleActive();

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
  }
}
} // namespace ctm