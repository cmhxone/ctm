#include "./cti_client.h"
#include "../util/ini_loader.h"
#include "./client_state.hpp"

#include <Poco/Net/SocketAddress.h>
#include <sstream>

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
    string port_key =
        client_state->isActive()
            ? is_secured ? "side.a.port.secure" : "side.a.port.plain"
        : is_secured ? "side.b.port.secure"
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
}

/**
 * @brief CTI 서버 접속
 * 
 */
void CTIClient::connect() noexcept {
    client_socket.connectNB(Poco::Net::SocketAddress{cti_server_host});

    if (!client_socket.poll(connection_timespan,
                            Poco::Net::Socket::SELECT_WRITE)) {
        // TODO: 채널 이벤트 전송
        return;
    }
}

/**
 * @brief CTI 서버 접속 해제
 * 
 */
void CTIClient::disconnect() noexcept {}
} // namespace ctm