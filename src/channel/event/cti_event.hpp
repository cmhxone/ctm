#pragma once

#include <algorithm>
#include <iterator>
#ifndef _CTM_CHANNEL_EVENT_CTI_EVENT_HPP_
#define _CTM_CHANNEL_EVENT_CTI_EVENT_HPP_

#include "../../cisco/common/message_type.hpp"
#include "./event.hpp"

#include <cstddef>
#include <vector>

namespace channel::event {
class CTIEvent : public Event {
public:
  /**
   * @brief Construct a new CTIEvent object
   *
   */
  CTIEvent(const std::vector<std::byte> &packet) : packet(packet) {}

  /**
   * @brief Destroy the CTIEvent object
   *
   */
  virtual ~CTIEvent() = default;

  /**
   * @brief Get the Event Type object
   *
   * @return constexpr EventType
   */
  virtual constexpr EventType getEventType() const override {
    return EventType::CTI_EVENT;
  }

  /**
   * @brief Get the Packet object
   *
   * @return const std::vector<std::byte>
   */
  const std::vector<std::byte> getPacket() const { return packet; }

  /**
   * @brief Get the Message Type object
   *
   * @return constexpr cisco::common::MessageType
   */
  constexpr cisco::common::MessageType getMessageType() const {
    std::vector<std::byte> message_type_packet{};
    std::move(packet.cbegin() + 4, packet.cbegin() + 8,
              std::back_inserter(message_type_packet));

    return cisco::common::deserialize<cisco::common::MessageType>(
        message_type_packet);
  }

protected:
private:
  std::vector<std::byte> packet;
};
} // namespace channel::event

#endif