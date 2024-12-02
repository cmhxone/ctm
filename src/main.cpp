#include "./channel/event_channel.hpp"
#include "./ctm/cti_client.h"
#include "channel/event/cti_error_event.hpp"
#include "channel/event/cti_event.hpp"

#include <chrono>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <thread>

using namespace std;

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::level_enum::debug);
    channel::EventChannel<channel::event::CTIEvent>::getInstance()->poll();
    channel::EventChannel<channel::event::CTIErrorEvent>::getInstance()->poll();

    ctm::CTIClient client{};
    channel::EventChannel<channel::event::CTIEvent>::getInstance()->subscribe(
        &client);
    channel::EventChannel<channel::event::CTIErrorEvent>::getInstance()
        ->subscribe(&client);

    client.connect();

    while (true) {
        this_thread::sleep_for(chrono::milliseconds{100});
    }

    spdlog::debug("Done");
    return EXIT_SUCCESS;
}