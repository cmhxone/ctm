#include "./cti_client.h"
#include "../channel/event/cti_error_event.hpp"
#include "../channel/event/cti_event.hpp"
#include "../channel/event_channel.hpp"
#include "../cisco/session/heartbeat_req.hpp"
#include "../cisco/session/open_req.hpp"
#include "../util/ini_loader.h"
#include "./client_state.hpp"

#include <Poco/Net/SocketAddress.h>

#include <algorithm>
#include <iterator>
#include <spdlog/spdlog.h>

#include <atomic>
#include <iomanip>
#include <ios>
#include <sstream>
#include <thread>

using namespace std;

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

  spdlog::debug("CTIClient constructed. cti_server_host: {}", cti_server_host);
}

/**
 * @brief CTI 서버 접속
 *
 */
void CTIClient::connect() noexcept {
  // 이미 접속 중이라면 다시 시도하지 않는다
  if (is_connected.load(memory_order_acquire)) {
    return;
  }

  client_socket.connectNB(Poco::Net::SocketAddress{cti_server_host});

  // 접속 실패시 CTI Error 를 채널에 전송한다
  if (!client_socket.poll(connection_timespan,
                          Poco::Net::Socket::SELECT_WRITE)) {
    spdlog::error("CTI Server connection failed. cti_server_host: {}",
                  cti_server_host);
    channel::EventChannel<channel::event::CTIErrorEvent>::getInstance()
        ->publish(channel::event::CTIErrorEvent{
            getCTIServerHost(),
            channel::event::CTIErrorEvent::CTIErrorType::CONNECTION_FAIL});
    return;
  }
  client_socket.setLinger(true, 3);
  client_socket.setNoDelay(true);

  // 접속 중 플래그 변경
  is_connected.store(true, memory_order_release);
  spdlog::debug("CTI Server connected. cti_server_host: {}", cti_server_host);

  // OPEN_REQ 메시지 전송 (Agent State Monitor 용 OPEN_REQ 메시지임)
  cisco::session::OpenReq open_req{};
  open_req.setInvokeID(1);
  open_req.setVersionNumber(24);
  open_req.setIdleTimeout(30);
  open_req.setCallMessageMask(0xffff'ffff);
  open_req.setServicesRequested(0x10);
  open_req.setAgentStateMask(0x3fff);
  open_req.setConfigMessageMask(15);
  open_req.setClientID("ctmonitor");
  open_req.setClientPW("");

  const vector<byte> packet = cisco::common::serialize(open_req);
  client_socket.sendBytes(packet.data(), packet.size());

  spdlog::debug("Sent OPEN_REQ message. cti_server_host: {}", cti_server_host);

  // 소켓 메시지 수신 스레드 생성
  thread t{[&]() {
    uint32_t invoke_id = 0;
    vector<byte> buffer{4'096};

    while (is_connected.load(memory_order_acquire)) {
      try {

        // 메시지 수신되는 건이 없다면 HEARTBEAT_REQ 를 주기적으로 전송
        if (!client_socket.poll(heartbeat_timespan,
                                Poco::Net::Socket::SELECT_READ)) {
          invoke_id++;
          invoke_id %= 1'000;

          cisco::session::HeartbeatReq heartbeat_req{};
          heartbeat_req.setInvokeID(invoke_id + 0x4F00'0000);

          const vector<byte> packet = cisco::common::serialize(heartbeat_req);
          client_socket.sendBytes(packet.data(), packet.size());

          continue;
        }

        // 수신된 패킷 디버그 로그 출력
        size_t length =
            client_socket.receiveBytes(buffer.data(), buffer.size());

        stringstream ss{};
        for (int i = 0; i < length; i++) {
          ss << std::setfill('0') << std::setw(2) << std::hex
             << static_cast<int32_t>(buffer.at(i)) << " ";

          if (i % 4 == 3) {
            ss << " ";
          }

          if (i % 16 == 15) {
            ss << "\n";
          }
        }
        spdlog::debug("Received packet. cti_server_host: {}\n{}",
                      cti_server_host, ss.str());

        // CTI 이벤트 배포
        std::vector<std::byte> received_packet{};
        std::move(buffer.cbegin(), buffer.cbegin() + length,
                  std::back_inserter(received_packet));

        channel::EventChannel<channel::event::CTIEvent>::getInstance()->publish(
            channel::event::CTIEvent{received_packet});
      } catch (const exception &e) {
        // 커넥션 실패 시 처리
        spdlog::error(
            "CTI Client exception. cti_server_host: {}, exception cause: {}",
            cti_server_host, e.what());
        is_connected.store(false, memory_order_release);

        channel::EventChannel<channel::event::CTIErrorEvent>::getInstance()
            ->publish(channel::event::CTIErrorEvent{
                getCTIServerHost(),
                channel::event::CTIErrorEvent::CTIErrorType::CONNECTION_LOST});
      }
    }
  }};

  t.detach();
}

/**
 * @brief 이벤트를 핸들링 한다
 *
 * @param event
 */
void CTIClient::handleEvent(const channel::event::Event *event) {
  switch (event->getEventType()) {
  case channel::event::EventType::CTI_EVENT:
    spdlog::debug("CTI_Event received. cti_server_host: {}, message_type: {}",
                  cti_server_host,
                  static_cast<uint32_t>(
                      dynamic_cast<const channel::event::CTIEvent *>(event)
                          ->getMessageType()));
    break;
  case channel::event::EventType::ERROR_EVENT:
    switch (dynamic_cast<const channel::event::ErrorEvent *>(event)
                ->getErrorType()) {
    case channel::event::ErrorType::CTI_ERROR:
      spdlog::debug(
          "CTI_Error_Event received. cti_server_host:{}, error_host: {}, "
          "cti_error_type: {}",
          cti_server_host,
          dynamic_cast<const channel::event::CTIErrorEvent *>(event)
              ->getErrorHost(),
          static_cast<int32_t>(
              dynamic_cast<const channel::event::CTIErrorEvent *>(event)
                  ->getCTIErrorType()));
      break;
    case channel::event::ErrorType::INTERNAL_ERROR:
      break;
    case channel::event::ErrorType::CLIENT_ERROR:
      break;
    }
    break;
  default:
    spdlog::debug("Unknown Event received. cti_server_host: {}",
                  cti_server_host);
    break;
  }
}

/**
 * @brief CTI 서버 접속 해제
 *
 */
void CTIClient::disconnect() noexcept {}
} // namespace ctm