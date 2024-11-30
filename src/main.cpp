#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
  Poco::Net::StreamSocket socket{Poco::Net::SocketAddress{"172.30.1.11:42027"}};

  socket.setNoDelay(true);
  socket.setLinger(true, 3);

  printf("Connected\n");

  vector<byte> buffer{};

  // Length
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x49);
  // Type
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x03);
  // InvokeID
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x01);
  // Protocol Version Number
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x18);
  // Idle Timeout
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x1E);
  // PeripheralID
  buffer.emplace_back((byte)0xff);
  buffer.emplace_back((byte)0xff);
  buffer.emplace_back((byte)0xff);
  buffer.emplace_back((byte)0xff);
  // Service Requested
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x10);
  // CallMessageMask
  buffer.emplace_back((byte)0xff);
  buffer.emplace_back((byte)0xff);
  buffer.emplace_back((byte)0xff);
  buffer.emplace_back((byte)0xff);
  // AgentStateMask
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x3f);
  buffer.emplace_back((byte)0xff);
  // Config Message Mask
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x06);
  // Reserved 1
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  // Reserved 2
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  // Reserved 3
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x00);
  // Floating Part
  // Client ID
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x01);
  // Length
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x0A);
  // ctmonitor
  buffer.emplace_back((byte)0x63);
  buffer.emplace_back((byte)0x74);
  buffer.emplace_back((byte)0x6D);
  buffer.emplace_back((byte)0x6F);
  buffer.emplace_back((byte)0x6E);
  buffer.emplace_back((byte)0x69);
  buffer.emplace_back((byte)0x74);
  buffer.emplace_back((byte)0x6F);
  buffer.emplace_back((byte)0x72);
  buffer.emplace_back((byte)0x00);
  // Client PW
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x02);
  // Length
  buffer.emplace_back((byte)0x00);
  buffer.emplace_back((byte)0x0B);
  // Comtec123!
  buffer.emplace_back((byte)0x43);
  buffer.emplace_back((byte)0x6F);
  buffer.emplace_back((byte)0x6D);
  buffer.emplace_back((byte)0x74);
  buffer.emplace_back((byte)0x65);
  buffer.emplace_back((byte)0x63);
  buffer.emplace_back((byte)0x31);
  buffer.emplace_back((byte)0x32);
  buffer.emplace_back((byte)0x33);
  buffer.emplace_back((byte)0x21);
  buffer.emplace_back((byte)0x00);

  printf("%02lx\n", buffer.size() - 8);

  size_t send_bytes = socket.sendBytes(buffer.data(), buffer.size());
  printf("Sent\n");

  while (true) {
    buffer.resize(1'024);
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