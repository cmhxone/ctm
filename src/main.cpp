#include "./channel/event/client_event.hpp"
#include "./channel/event/cti_error_event.hpp"
#include "./channel/event/cti_event.hpp"
#include "./channel/event_channel.hpp"
#include "./ctm/bridge/message_bridge.hpp"
#include "./ctm/ctm.h"
#include "channel/event/bridge_event.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <cstdlib>
#include <thread>

using namespace std;
using namespace channel::event;

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::level_enum::debug);

  channel::EventChannel<CTIEvent>::getInstance()->poll();
  channel::EventChannel<CTIErrorEvent>::getInstance()->poll();
  channel::EventChannel<ClientEvent>::getInstance()->poll();
  channel::EventChannel<BridgeEvent>::getInstance()->poll();

  ctm::CTM ctm{};

  channel::EventChannel<CTIEvent>::getInstance()->subscribe(
      ctm::bridge::MessageBridge::getInstance());
  channel::EventChannel<ClientEvent>::getInstance()->subscribe(
      ctm::bridge::MessageBridge::getInstance());
  channel::EventChannel<CTIErrorEvent>::getInstance()->subscribe(&ctm);

  while (true) {
    this_thread::sleep_for(chrono::milliseconds{100});
  }

  spdlog::debug("Done");
  return EXIT_SUCCESS;
}