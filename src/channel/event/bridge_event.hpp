#pragma once

#ifndef _CTM_CHANNEL_EVENT_BRIDGE_EVENT_HPP_
#define _CTM_CHANNEL_EVENT_BRIDGE_EVENT_HPP_

#include "./event.hpp"

#include <cstdint>
#include <string>

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

  /**
   * @brief 브릿지 이벤트 유형
   *
   */
  enum class BridgeEventType : std::uint8_t {
    NONE,
    QUERY_AGENT,
  };

  /**
   * @brief 브릿지 이벤트 메시지
   *
   */
  struct BridgeEventMessage {
    BridgeEventType type;
    std::string message;
  };

public:
  /**
   * @brief Construct a new Bridge Event object
   *
   * @param destination
   */
  BridgeEvent(const BridgeEventDestination &destination,
              const BridgeEventMessage &message)
      : destination(destination), message(message) {}

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

  /**
   * @brief Get the Bridge Event Message object
   *
   * @return const BridgeEventMessage&
   */
  virtual const BridgeEventMessage &getBridgeEventMessage() const {
    return message;
  }

protected:
private:
  BridgeEventDestination destination;
  BridgeEventMessage message;
};
} // namespace channel::event

#endif