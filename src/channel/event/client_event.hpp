#pragma once

#ifndef _CTM_CHANNEL_EVENT_CLIENT_EVENT_HPP_
#define _CTM_CHANNEL_EVENT_CLIENT_EVENT_HPP_

#include "event.hpp"

#include <cstddef>
#include <vector>

namespace channel::event {
class ClientEvent : public Event {
public:
  /**
   * @brief Construct a new Client Event object
   *
   * @param packet
   */
  ClientEvent(const std::vector<std::byte> &packet) : packet(packet) {}

  /**
   * @brief Destroy the Client Event object
   *
   */
  virtual ~ClientEvent() = default;

  /**
   * @brief Get the Packet object
   *
   * @return const std::vector<std::byte>
   */
  const std::vector<std::byte> getPacket() const { return packet; }

  /**
   * @brief Get the Event Type object
   *
   * @return constexpr EventType
   */
  virtual constexpr EventType getEventType() const override {
    return EventType::CLIENT_EVENT;
  }

protected:
private:
  std::vector<std::byte> packet;
};
} // namespace channel::event

#endif