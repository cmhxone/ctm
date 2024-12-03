#pragma once

#ifndef _CTM_CHANNEL_EVENT_CHANNEL_HPP_
#define _CTM_CHANNEL_EVENT_CHANNEL_HPP_

#include "../template/singleton.hpp"
#include "./event/event.hpp"
#include "./subscriber.hpp"

#include <spdlog/spdlog.h>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace channel {
/**
 * @brief 이벤트 채널
 *
 */
template <event::DerivedEvent T>
class EventChannel : public tmpl::Singleton<EventChannel<T>> {
public:
  /**
   * @brief Construct a new Event Channel object
   *
   */
  EventChannel() {}
  /**
   * @brief Destroy the Event Channel object
   *
   */
  virtual ~EventChannel() = default;

  /**
   * @brief 이벤트 채널 폴링
   *
   */
  void poll() noexcept {
    // 이미 실행중인 경우 추가 스레드 생성하지 않음
    if (isLaunched()) {
      return;
    }

    is_launched.store(true, std::memory_order_release);

    // Polling 스레드 생성
    std::thread t{[&]() {
      spdlog::debug("Event channel polling thread launched");

      while (isLaunched()) {
          std::unique_lock lk{channel_mtx};

          channel_cv.wait(lk, [&]() { return !event_queue.empty(); });

          const event::Event *event = &event_queue.front();

          // 구독자가 이벤트를 처리한다
          subscriber_mtx.lock();
          for (Subscriber *subscriber : subscribers) {
            subscriber->handleEvent(event);
          }
          subscriber_mtx.unlock();

          event_queue.pop();

          lk.unlock();
        }
    }};

    t.detach();
  }

  /**
   * @brief 이벤트 채널 폴링 종료
   *
   */
  void stop() noexcept { is_launched.store(false, std::memory_order_release); }

  /**
   * @brief 이벤트 채널 이벤트 배포
   *
   * @param event
   */
  void publish(const T &event) noexcept {
    channel_mtx.lock();
    event_queue.push(event);
    channel_mtx.unlock();

    channel_cv.notify_one();
  }

  /**
   * @brief 이벤트 채널 구독
   *
   * @param subscriber
   */
  void subscribe(Subscriber *subscriber) {
    subscriber_mtx.lock();
    subscribers.push_back(subscriber);
    subscriber_mtx.unlock();
  }

protected:
  std::queue<T> event_queue{};
  std::recursive_mutex channel_mtx{};
  std::condition_variable_any channel_cv{};
  std::atomic_bool is_launched{false};

  std::vector<Subscriber *> subscribers{};
  std::recursive_mutex subscriber_mtx{};

  /**
   * @brief 폴링 스레드 실행 여부 반환
   *
   * @return true
   * @return false
   */
  constexpr bool isLaunched() const {
    return is_launched.load(std::memory_order_acquire);
  }

private:
};
} // namespace channel

#endif
