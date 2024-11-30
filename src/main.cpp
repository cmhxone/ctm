#include <cstdlib>

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <vector>

#include "./cisco/session/open_req.h"

using namespace std;
using namespace cisco::common;

int main(int argc, char **argv) {
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

    for (int i = 0; const std::byte &b : packet) {
        printf("%02lx ", b);

        if (i % 4 == 3) {
            printf(" ");
        }

        if (i++ % 16 == 15) {
            printf("\n");
        }
    }
    printf("\n");

    Poco::Net::StreamSocket socket{
        Poco::Net::SocketAddress{"172.30.1.11:42027"}};

    socket.setNoDelay(true);
    socket.setLinger(true, 3);

    printf("Connected\n");

    printf("%02lx\n", packet.size() - 8);

    size_t send_bytes = socket.sendBytes(packet.data(), packet.size());
    printf("Sent\n");

    vector<byte> buffer{1'024};
    while (true) {
        size_t recv_bytes = socket.receiveBytes(buffer.data(), buffer.size());
        if (recv_bytes == 0) {
            break;
        }

        printf("Received\n");

        for (int i = 0; i < recv_bytes; i++) {
            printf("%02x ", buffer.at(i));
            if (i % 4 == 3) {
                printf(" ");
            }
            if (i % 16 == 15) {
                printf("\n");
            }
        }
        printf("\n");
    }

    printf("Done");
    return EXIT_SUCCESS;
}