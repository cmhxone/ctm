#include "./channel/event/client_event.hpp"
#include "./channel/event/cti_error_event.hpp"
#include "./channel/event/cti_event.hpp"
#include "./channel/event_channel.hpp"
#include "./ctm/bridge/message_bridge.hpp"
#include "./ctm/ctm.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <cstdlib>
#include <thread>

using namespace std;
using namespace channel::event;

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::level_enum::debug);

  ctm::CTM ctm{};

  channel::EventChannel<CTIEvent>::getInstance()->subscribe(
      ctm::bridge::MessageBridge::getInstance());
  channel::EventChannel<ClientEvent>::getInstance()->subscribe(
      ctm::bridge::MessageBridge::getInstance());
  channel::EventChannel<CTIErrorEvent>::getInstance()->subscribe(&ctm);

  while (true) {
    this_thread::sleep_for(chrono::milliseconds{100});
  }

  channel::EventChannel<CTIEvent>::getInstance()->unsubscribe(
      ctm::bridge::MessageBridge::getInstance());
  channel::EventChannel<ClientEvent>::getInstance()->unsubscribe(
      ctm::bridge::MessageBridge::getInstance());
  channel::EventChannel<CTIErrorEvent>::getInstance()->unsubscribe(&ctm);

  spdlog::debug("Done");
  return EXIT_SUCCESS;
}