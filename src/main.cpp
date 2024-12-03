#include "./channel/event_channel.hpp"
#include "channel/event/cti_error_event.hpp"
#include "channel/event/cti_event.hpp"

#include "./ctm/ctm.h"

#include <chrono>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <thread>

using namespace std;
using namespace channel::event;

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::level_enum::debug);

  channel::EventChannel<CTIEvent>::getInstance()->poll();
  channel::EventChannel<CTIErrorEvent>::getInstance()->poll();

  ctm::CTM ctm{};

  channel::EventChannel<CTIEvent>::getInstance()->subscribe(&ctm);
  channel::EventChannel<CTIErrorEvent>::getInstance()->subscribe(&ctm);

  while (true) {
    this_thread::sleep_for(chrono::milliseconds{100});
  }

  spdlog::debug("Done");
  return EXIT_SUCCESS;
}