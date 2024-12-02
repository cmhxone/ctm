#include "./command_event.h"

#include <spdlog/spdlog.h>

namespace channel::event {
CommandEvent::CommandEvent() {}

void CommandEvent::handleEvent() { spdlog::debug("Handled"); }
} // namespace channel