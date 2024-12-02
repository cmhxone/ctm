#include "./channel/event_channel.hpp"
#include "./ctm/cti_client.h"
#include "channel/event/command_event.h"

#include <chrono>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <thread>

using namespace std;

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::level_enum::debug);

  ctm::CTIClient client{};

  channel::EventChannel<channel::event::CommandEvent>::getInstance()->subscribe(
      &client);

  client.connect();

  while (true) {
    this_thread::sleep_for(chrono::milliseconds{100});
  }

  spdlog::debug("Done");
  return EXIT_SUCCESS;
}