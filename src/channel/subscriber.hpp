#pragma once

#ifndef _CTM_CHANNEL_SUBSCRIBER_HPP_
#define _CTM_CHANNEL_SUBSCRIBER_HPP_

#include "./event/event.hpp"

namespace channel {
class Subscriber {
public:
  virtual void handleEvent(const event::Event &event) = 0;

protected:
private:
};
} // namespace channel

#endif