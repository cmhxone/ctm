#pragma once

#ifndef _CTM_CHANNEL_EVENT_CHANNEL_HPP_
#define _CTM_CHANNEL_EVENT_CHANNEL_HPP_

#include "../template/singleton.hpp"
#include "./event.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace channel {
/**
 * @brief 이벤트 채널
 *
 */
template <DerivedEvent T>
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
     * @brief 이벤트 채널 구독
     *
     */
    void subscribe() noexcept {
        std::thread t{[&]() {
            while (true) {
                std::unique_lock lk{channel_mtx};

                channel_cv.wait(lk, [&]() { return !event_queue.empty(); });

                event_queue.front().handleEvent();
                event_queue.pop();

                lk.unlock();
            }
        }};

        t.detach();
    }

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

  protected:
    std::queue<T> event_queue{};
    std::mutex channel_mtx{};
    std::condition_variable channel_cv{};

  private:
};
} // namespace channel

#endif
