#include "./channel/command_event.h"
#include "./channel/event_channel.hpp"
#include "./cisco/session/heartbeat_req.h"
#include "./cisco/session/open_req.h"
#include "./util/ini_loader.h"

#include <Poco/Exception.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Timespan.h>

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <thread>
#include <vector>

using namespace std;
using namespace cisco::common;

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::level_enum::debug);

    channel::EventChannel<channel::CommandEvent>::getInstance()->subscribe();

    channel::EventChannel<channel::CommandEvent>::getInstance()->publish(channel::CommandEvent{});
    channel::EventChannel<channel::CommandEvent>::getInstance()->publish(channel::CommandEvent{});
    channel::EventChannel<channel::CommandEvent>::getInstance()->publish(channel::CommandEvent{});
    channel::EventChannel<channel::CommandEvent>::getInstance()->publish(channel::CommandEvent{});
    channel::EventChannel<channel::CommandEvent>::getInstance()->publish(channel::CommandEvent{});
    channel::EventChannel<channel::CommandEvent>::getInstance()->publish(channel::CommandEvent{});
    channel::EventChannel<channel::CommandEvent>::getInstance()->publish(channel::CommandEvent{});
    channel::EventChannel<channel::CommandEvent>::getInstance()->publish(channel::CommandEvent{});
    channel::EventChannel<channel::CommandEvent>::getInstance()->publish(channel::CommandEvent{});

    const auto *loader = util::IniLoader::getInstance();
    const string side_a_ip = loader->get("cti", "side.a.ip", "localhost"s);
    spdlog::debug(side_a_ip);

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

    const auto packet = serialize(open_req);

    {
        stringstream ss{};
        ss << "\n";

        for (int i = 0; const std::byte &b : packet) {
            ss << std::setw(2) << std::setfill('0') << std::hex
               << static_cast<int32_t>(b) << " ";

            if (i % 4 == 3) {
                ss << " ";
            }

            if (i++ % 16 == 15) {
                ss << "\n";
            }
        }
        spdlog::debug(ss.str());
    }

    Poco::Net::StreamSocket socket{
        Poco::Net::SocketAddress{"172.30.1.11:42027"}};
    socket.setBlocking(false);

    Poco::Timespan timeout{5'000'000};
    if (!socket.poll(timeout, Poco::Net::Socket::SELECT_WRITE)) {
        spdlog::error("Socket connection timed out.");
        return EXIT_FAILURE;
    }

    socket.setNoDelay(true);
    socket.setLinger(true, 3);

    spdlog::debug("Connected");

    size_t send_bytes = socket.sendBytes(packet.data(), packet.size());
    spdlog::debug("Sent");

    uint32_t invoke_id = 2;
    vector<byte> buffer{1'024};
    while (true) {
        if (!socket.poll(timeout, Poco::Net::Socket::SELECT_READ)) {
            cisco::session::HeartbeatReq heartbeat_req{};
            heartbeat_req.setInvokeID(invoke_id++);

            const auto packet = serialize(heartbeat_req);

            socket.sendBytes(packet.data(), packet.size());
            spdlog::debug("Sent heartbeat");
            continue;
        }

        size_t recv_bytes = socket.receiveBytes(buffer.data(), buffer.size());
        spdlog::debug("Received");
        {
            stringstream ss{};
            ss << "\n";

            for (int i = 0; i < recv_bytes; i++) {
                ss << std::setw(2) << std::setfill('0') << std::hex
                   << static_cast<int32_t>(buffer.at(i)) << " ";
                if (i % 4 == 3) {
                    ss << " ";
                }
                if (i % 16 == 15) {
                    ss << "\n";
                }
            }
            spdlog::debug(ss.str());
        }
    }

    spdlog::debug("Done");
    return EXIT_SUCCESS;
}