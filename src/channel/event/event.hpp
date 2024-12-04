#pragma once

#ifndef _CTM_CHANNEL_EVENT_EVENT_HPP_
#define _CTM_CHANNEL_EVENT_EVENT_HPP_

#include <cstdint>
#include <type_traits>

namespace channel::event {
/**
 * @brief 이벤트 유형
 *
 */
enum class EventType : std::int32_t {
  CTI_EVENT,    // CTI 서버로부터 발생하는 이벤트
  ERROR_EVENT,  // 전역 오류 이벤트
  BRIDGE_EVENT, // 메시지 브릿지로부터 발생하는 이벤트
  CLIENT_EVENT, // CTM 클라이언트로부터 발생하는 이벤트
};

/**
 * @brief 채널에서 처리할 이벤트 추상 클래스
 *
 */
class Event {
public:
  /**
   * @brief Construct a new Event object
   *
   */
  Event() {};

  /**
   * @brief Destroy the Event object
   *
   */
  virtual ~Event() = default;

  /**
   * @brief Get the Event Type object
   *
   * @return const EventType
   */
  virtual constexpr EventType getEventType() const = 0;

protected:
private:
};

template <typename T>
concept DerivedEvent = std::is_base_of<Event, T>::value;

} // namespace channel::event

#endif