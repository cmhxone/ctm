#pragma once

#ifndef _CTM_CHANNEL_EVENT_ERROR_EVENT_HPP_
#define _CTM_CHANNEL_EVENT_ERROR_EVENT_HPP_

#include "event.hpp"

namespace channel::event {
/**
 * @brief 오류 타입 이늄
 *
 */
enum class ErrorType {
  CTI_ERROR,
  INTERNAL_ERROR,
  CLIENT_ERROR,
};

/**
 * @brief 오류 이벤트
 *
 */
class ErrorEvent : public Event {
public:
  /**
   * @brief Construct a new Error Event object
   *
   * @param error_type
   */
  ErrorEvent(const ErrorType &error_type) : error_type(error_type) {}

  /**
   * @brief Destroy the Error Event object
   *
   */
  virtual ~ErrorEvent() = default;

  /**
   * @brief Get the Event Type object
   *
   * @return constexpr EventType
   */
  virtual constexpr EventType getEventType() const override {
    return EventType::ERROR_EVENT;
  }

  /**
   * @brief Get the Error Type object
   *
   * @return constexpr ErrorType
   */
  virtual constexpr ErrorType getErrorType() const = 0;

protected:
  ErrorType error_type;

private:
};
} // namespace channel::event

#endif