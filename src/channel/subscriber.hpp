#pragma once

#ifndef _CTM_CHANNEL_SUBSCRIBER_HPP_
#define _CTM_CHANNEL_SUBSCRIBER_HPP_

#include "./event/event.hpp"

namespace channel {
/**
 * @brief 채널 구독자 추상 클래스
 *
 */
class Subscriber {
public:
  /**
   * @brief 이벤트 핸들링 추상 메소드
   *
   * @param event
   */
  virtual void handleEvent(const event::Event *event) = 0;

protected:
private:
};
} // namespace channel

#endif