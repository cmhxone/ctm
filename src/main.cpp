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

  while (true) {
    this_thread::sleep_for(chrono::milliseconds{100});
  }

  spdlog::debug("Done");
  return EXIT_SUCCESS;
}