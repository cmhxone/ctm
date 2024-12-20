#include "./cti_client.h"
#include "../channel/event/bridge_event.hpp"
#include "../channel/event/cti_error_event.hpp"
#include "../channel/event/cti_event.hpp"
#include "../channel/event/event.hpp"
#include "../channel/event_channel.hpp"
#include "../cisco/control/query_agent_state_req.hpp"
#include "../cisco/session/heartbeat_req.hpp"
#include "../cisco/session/open_req.hpp"
#include "../util/ini_loader.h"
#include "./client_state.hpp"

#include <Poco/AutoPtr.h>
#include <Poco/NObserver.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/SocketNotification.h>
#include <spdlog/spdlog.h>

#include <atomic>
#include <chrono>
#include <iomanip>
#include <ios>
#include <regex>
#include <sstream>
#include <thread>
#include <vector>

using namespace std;
using namespace channel;

namespace ctm {
/**
 * @brief Construct a new CTIClient::CTIClient object
 *
 */
CTIClient::CTIClient() {
  const ClientState *client_state = ClientState::getInstance();
  const util::IniLoader *ini_loader = util::IniLoader::getInstance();

  // TLS 프로토콜 사용 여부
  bool is_secured = ini_loader->get("cti", "protocol.secure", false);

  // Active/Standby Plain/Secure 구분
  string ip_key = client_state->isActive() ? "side.a.ip" : "side.b.ip";
  string port_key = client_state->isActive() ? is_secured ? "side.a.port.secure"
                                                          : "side.a.port.plain"
                    : is_secured             ? "side.b.port.secure"
                                             : "side.b.port.plain";

  // CG 접속정보 저장 (기본값: localhost:42027)
  stringstream ss{};
  ss << ini_loader->get("cti", ip_key, "localhost"s) << ":"
     << ini_loader->get("cti", port_key, 42027);
  this->cti_server_host = ss.str();

  // Timeout 설정
  this->connection_timespan =
      ini_loader->get("cti", "timeout.connection", 5'000) * 1'000;
  this->heartbeat_timespan =
      ini_loader->get("cti", "timeout.heartbeat", 5'000) * 1'000;
  client_socket_reactor.setTimeout(connection_timespan);

  EventChannel<event::BridgeEvent>::getInstance()->subscribe(this);

  spdlog::info("CTIClient constructed. cti_server_host: {}", cti_server_host);
}

/**
 * @brief Destroy the CTIClient::CTIClient object
 *
 */
CTIClient::~CTIClient() {
  EventChannel<event::BridgeEvent>::getInstance()->unsubscribe(this);
  client_socket_reactor.stop();
  reactor_thread.join();
}

/**
 * @brief CTI 서버 접속
 *
 */
void CTIClient::connect() noexcept {
  // 초기화 상태가 아니라면 접속 시도하지 않는다
  if (getCurrentState() != FiniteState::INITIALIZED) {
    return;
  }

  // 접속 시도하고 현재 FSM 값을 변경
  try {
    client_socket.connect(Poco::Net::SocketAddress{cti_server_host},
                          connection_timespan);
  } catch (const exception &e) {
    spdlog::error(
        "Unabled to connect CTI Server. cti_server_host: {}, reason: {}",
        getCTIServerHost(), e.what());
    EventChannel<event::CTIErrorEvent>::getInstance()->publish(
        event::CTIErrorEvent{
            getCTIServerHost(),
            event::CTIErrorEvent::CTIErrorType::CONNECTION_FAIL});
    return;
  }

  // 소켓 옵션 설정
  client_socket.setBlocking(false);
  client_socket.setSendTimeout(heartbeat_timespan);
  client_socket.setReceiveTimeout(heartbeat_timespan);
  current_state.store(FiniteState::CONNECTING, memory_order::release);

  // Reactor 콜백 등록
  client_socket_reactor.addEventHandler(
      client_socket,
      Poco::NObserver<CTIClient, Poco::Net::ReadableNotification>{
          *this, &CTIClient::onReadableNotification});
  client_socket_reactor.addEventHandler(
      client_socket, Poco::NObserver<CTIClient, Poco::Net::ErrorNotification>{
                         *this, &CTIClient::onErrorNotification});
  client_socket_reactor.addEventHandler(
      client_socket,
      Poco::NObserver<CTIClient, Poco::Net::ShutdownNotification>{
          *this, &CTIClient::onShutdownNotification});

  // Poco 스레드를 사용해 처리해야 예외가 발생하지 않는다.
  reactor_thread.start(client_socket_reactor);

  // 접속 중 플래그 변경
  current_state.store(FiniteState::CONNECTED, memory_order::release);
  spdlog::info("CTI Server connected. cti_server_host: {}", cti_server_host);

  // 소켓 옵션 설정
  client_socket.setLinger(true, 3);
  client_socket.setNoDelay(true);

  // OPEN_REQ 메시지 전송 (Agent State Monitor 용 OPEN_REQ 메시지임)
  cisco::session::OpenReq open_req{};
  open_req.setInvokeID(getInvokeID());
  open_req.setVersionNumber(24);
  open_req.setIdleTimeout(300);
  open_req.setCallMessageMask(0xffff'ffff);
  open_req.setServicesRequested(0x80 | 0x10 | 0x04);
  open_req.setAgentStateMask(0x3fff);
  open_req.setConfigMessageMask(0);
  open_req.setPeripheralID(5000);
  open_req.setClientID("ctmonitor");
  open_req.setClientPW("");

  const vector<byte> packet = cisco::common::serialize(open_req);
  client_socket.sendBytes(packet.data(), packet.size());

  // HeartBeat 전송 스레드 실행
  thread heartbeat_thread{[&]() {
    while (getCurrentState() == FiniteState::CONNECTED) {
      addInvokeID();

      cisco::session::HeartbeatReq heartbeat_req{};
      heartbeat_req.setInvokeID(getInvokeID());

      const vector<byte> packet = cisco::common::serialize(heartbeat_req);
      client_socket.sendBytes(packet.data(), packet.size());

      spdlog::info("Sent HEARTBEAT_REQ. cti_server_host: {}, invoke_id: {}",
                   cti_server_host, heartbeat_req.getInvokeID());

      // 스레드 sleep을 위로 올리는 경우 current_state 값을 예전 값으로
      // 처리온다. sleep 한 사이에 소켓이 종료 되었을 수 있으니 루프문 마지막에
      // sleep을 걸어준다.
      this_thread::sleep_for(
          chrono::milliseconds{heartbeat_timespan.totalMilliseconds()});
    }
  }};
  heartbeat_thread.detach();

  spdlog::info("Sent OPEN_REQ message. cti_server_host: {}, invoke_id: {}",
               cti_server_host, open_req.getInvokeID());
}

/**
 * @brief CTI 서버 접속 해제
 *
 */
void CTIClient::disconnect() noexcept {
  spdlog::info("CTI Server disconnected. cti_server_host: {}", cti_server_host);
  current_state.store(FiniteState::FINISHED, memory_order::release);
  client_socket_reactor.stop();
}

/**
 * @brief 읽을 데이터가 있을 경우
 *
 * @param notification
 */
void CTIClient::onReadableNotification(
    const Poco::AutoPtr<Poco::Net::ReadableNotification> &notification) {

  // 수신된 패킷 디버그 로그 출력
  size_t length =
      client_socket.receiveBytes(receive_buffer.data(), receive_buffer.size());

  // 들어온 메시지 길이가 0인 경우 소켓이 끊어졌다 판단
  if (length < 0) {
    current_state.store(FiniteState::FINISHED, memory_order::release);
    channel::EventChannel<channel::event::CTIErrorEvent>::getInstance()
        ->publish(channel::event::CTIErrorEvent(
            getCTIServerHost(),
            channel::event::CTIErrorEvent::CTIErrorType::CONNECTION_LOST));
    client_socket.close();
    return;
  }

  stringstream ss{};
  for (int i = 0; i < length; i++) {
    ss << std::setfill('0') << std::setw(2) << std::hex
       << static_cast<int32_t>(receive_buffer.at(i)) << " ";

    if (i % 4 == 3) {
      ss << " ";
    }

    if (i % 16 == 15) {
      ss << "\n";
    }
  }
  spdlog::debug("Received packet. cti_server_host: {}\n{}", cti_server_host,
                ss.str());

  // 메시지 헤더 MHDR 정보를 이용해, 여러 패킷이 동시에 수신된 경우 분리하여
  // 이벤트를 배포한다
  size_t packet_index = 0;
  vector<byte> seperator_buffer{};
  while (packet_index < length) {
    // 분리용 버퍼에 수신 버퍼 복사
    seperator_buffer = vector<byte>{receive_buffer.cbegin() + packet_index,
                                    receive_buffer.cend()};
    // 메시지 헤더 분리
    cisco::common::MHDR mhdr =
        cisco::common::deserialize<cisco::common::MHDR>(seperator_buffer);
    // 현재 처리중 패킷 위치 누산 (8 = MHDR 길이)
    packet_index += mhdr.getMessageLength() + 8;
    // CTI 이벤트 배포
    channel::EventChannel<channel::event::CTIEvent>::getInstance()->publish(
        channel::event::CTIEvent{seperator_buffer});
  }
}

/**
 * @brief 오류 알림 수신
 *
 * @param notification
 */
void CTIClient::onErrorNotification(
    const Poco::AutoPtr<Poco::Net::ErrorNotification> &notification) {
  // 이미 오류 핸들링 된 경우는 처리하지 않는다
  if (getCurrentState() == FiniteState::FINISHED) {
    return;
  }

  // 오류 핸들링
  current_state.store(FiniteState::FINISHED, memory_order::release);
  client_socket_reactor.stop();

  // 오류 메시지 전송
  EventChannel<event::CTIErrorEvent>::getInstance()->publish(
      event::CTIErrorEvent{
          getCTIServerHost(),
          event::CTIErrorEvent::CTIErrorType::CONNECTION_LOST});

  spdlog::warn("Error notified from CTI Server. cti_server_host: {}",
               cti_server_host);
}

/**
 * @brief 종료 알림 수신
 *
 * @param notification
 */
void CTIClient::onShutdownNotification(
    const Poco::AutoPtr<Poco::Net::ShutdownNotification> &notification) {
  current_state.store(FiniteState::FINISHED, memory_order::release);
  client_socket_reactor.stop();
}

/**
 * @brief 이벤트 핸들러
 *
 * @param event
 */
void CTIClient::handleEvent(const event::Event *event) {
  switch (event->getEventType()) {
  case event::EventType::BRIDGE_EVENT: {
    const event::BridgeEvent *bridge_event =
        dynamic_cast<const event::BridgeEvent *>(event);

    // CTI에게 전달된 이벤트만 핸들링
    if (bridge_event->getDestination() !=
        event::BridgeEvent::BridgeEventDestination::CTI) {
      return;
    }

    // 브릿지 이벤트에 따라 처리
    switch (bridge_event->getBridgeEventMessage().type) {
    case event::BridgeEvent::BridgeEventType::NONE:
      break;
    case event::BridgeEvent::BridgeEventType::QUERY_AGENT: {
      std::regex regexp{R"regex(([0-9]*)\-([0-9]*))regex"};
      std::smatch match{};

      std::string buffer{};
      buffer.resize(bridge_event->getBridgeEventMessage().message.size());
      std::memcpy(buffer.data(),
                  bridge_event->getBridgeEventMessage().message.data(),
                  bridge_event->getBridgeEventMessage().message.size());

      std::regex_match(buffer, match, regexp);

      cisco::control::QueryAgentStateReq query_agent_state_req{};
      addInvokeID();
      query_agent_state_req.setInvokeID(getInvokeID());
      query_agent_state_req.setPeripheralID(std::stoi(match[1].str()));
      query_agent_state_req.setAgentID(match[2].str());

      // Query Agent State 커맨드를 전송한다
      client_socket.sendBytes(
          cisco::common::serialize(query_agent_state_req).data(),
          cisco::common::serialize(query_agent_state_req).size());

      spdlog::info("Sent QUERY_AGENT_STATE_REQ. cti_server_host: {}, "
                   "invoke_id: {}, agent_id: {}",
                   cti_server_host, query_agent_state_req.getInvokeID(),
                   query_agent_state_req.getAgentID().value());
    } break;
    case event::BridgeEvent::BridgeEventType::BROADCAST_AGENT_STATE:
      break;
    }
  } break;
  default:
    break;
  }
}
} // namespace ctm
