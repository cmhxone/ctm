#pragma once

#ifndef _CTM_CTM_CLIENT_STATE_HPP_
#define _CTM_CTM_CLIENT_STATE_HPP_

#include "../template/singleton.hpp"

#include <atomic>
#include <cstdint>

namespace ctm {
class ClientState : public tmpl::Singleton<ClientState> {
  public:
    /**
     * @brief Construct a new Client State object
     *
     */
    ClientState() {}

    /**
     * @brief Destroy the Client State object
     *
     */
    virtual ~ClientState() = default;

    /**
     * @brief
     *
     * @return true
     * @return false
     */
    constexpr bool isActive() const {
        return this->is_active.load(std::memory_order_acquire);
    }

    /**
     * @brief Get the Retry Count object
     *
     * @return constexpr std::uint8_t
     */
    constexpr std::uint8_t getRetryCount() const {
        return this->retry_count.load(std::memory_order_acquire);
    }

    /**
     * @brief
     *
     */
    void toggleActive() {
        bool current = isActive();
        this->is_active.store(!current, std::memory_order_release);
    }

    /**
     * @brief
     *
     */
    void addRetryCount() {
        std::int8_t current = getRetryCount();
        this->retry_count.store(current + 1, std::memory_order_release);
    }

    /**
     * @brief
     *
     */
    void resetRetryCount() {
        this->retry_count.store(0, std::memory_order_release);
    }

  protected:
  private:
    std::atomic_bool is_active = true;
    std::atomic_uint8_t retry_count = 0;
};
} // namespace ctm

#endif