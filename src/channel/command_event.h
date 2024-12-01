#pragma once

#ifndef _CTM_CHANNEL_COMMAND_EVENT_H_
#define _CTM_CHANNEL_COMMAND_EVENT_H_

#include "./event.hpp"

namespace channel {
class CommandEvent : public Event {
  public:
    CommandEvent();
    virtual ~CommandEvent() = default;

    virtual void handleEvent() override;

  protected:
  private:
};
} // namespace channel

#endif