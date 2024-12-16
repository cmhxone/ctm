#pragma once

#ifndef _CTM_CHANNEL_EVENT_BRIDGE_EVENT_HPP_
#define _CTM_CHANNEL_EVENT_BRIDGE_EVENT_HPP_

#include "../../ctm/message/message.hpp"
#include "./event.hpp"

#include <cstdint>
#include <memory>

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
   * @brief Construct a new Bridge Event object
   *
   * @param destination
   * @param message
   */
  BridgeEvent(const BridgeEventDestination &destination,
              const std::shared_ptr<ctm::message::Message> message)
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
   * @brief Get the Message object
   *
   * @return std::shared_ptr<ctm::message::Message>
   */
  virtual std::shared_ptr<ctm::message::Message> getMessage() const {
    return message;
  }

  /**
   * @brief Set the Message object
   *
   * @param message
   */
  virtual void
  setMessage(const std::shared_ptr<ctm::message::Message> message) {
    this->message = message;
  }

protected:
private:
  BridgeEventDestination destination;
  std::shared_ptr<ctm::message::Message> message;
};
} // namespace channel::event

#endif