#pragma once

#ifndef _CTM_CHANNEL_EVENT_CHANNEL_HPP_
#define _CTM_CHANNEL_EVENT_CHANNEL_HPP_

#include "../template/singleton.hpp"
#include "./event.hpp"

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
        // 이미 실행중인 경우 추가 스레드 생성하지 않음
        if (is_launched.load(std::memory_order_acquire)) {
            return;
        }

        is_launched.store(true, std::memory_order_release);

        // Polling 스레드 생성 
        std::thread t{[&]() {
            spdlog::debug("Event channel polling thread launched");

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
    std::atomic_bool is_launched{false};

  private:
};
} // namespace channel

#endif
