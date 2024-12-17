#pragma once

#ifndef _CTM_CHANNEL_EVENT_BRIDGE_EVENT_HPP_
#define _CTM_CHANNEL_EVENT_BRIDGE_EVENT_HPP_

#include "./event.hpp"

#include <cstdint>

namespace channel::event {
class BridgeEvent : public Event {
public:
  /**
   * @brief 브릿지 메시지 수신 대상
   *
   */
  enum class BridgeEventDestination : std::uint8_t {
    CTI,
    CLIENT,
  };

public:
  /**
   * @brief Construct a new Bridge Event object
   *
   * @param destination
   */
  BridgeEvent(const BridgeEventDestination &destination)
      : destination(destination) {}

  /**
   * @brief Destroy the Bridge Event object
   *
   */
  virtual ~BridgeEvent() = default;

  /**
   * @brief Get the Event Type object
   *
   * @return constexpr EventType
   */
  virtual constexpr EventType getEventType() const override {
    return EventType::BRIDGE_EVENT;
  }

  /**
   * @brief Get the Destination object
   *
   * @return constexpr BridgeEventDestination
   */
  virtual constexpr BridgeEventDestination getDestination() const {
    return destination;
  }

protected:
private:
  BridgeEventDestination destination;
};
} // namespace channel::event

#endif