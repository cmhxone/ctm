#pragma once

#ifndef _CTM_CHANNEL_EVENT_EVENT_HPP_
#define _CTM_CHANNEL_EVENT_EVENT_HPP_

#include <type_traits>

namespace channel::event {
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

  protected:
  private:
};

template <typename T>
concept DerivedEvent = std::is_base_of<Event, T>::value;

} // namespace channel

#endif