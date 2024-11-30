#pragma once

#ifndef _CTM_CISCO_COMMON_MHDR_HPP_
#define _CTM_CISCO_COMMON_MHDR_HPP_

#include "./message_type.hpp"
#include "./serializable.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <vector>

namespace cisco::common {

/**
 * @brief Message Header
 *
 */
class MHDR {
  public:
    /**
     * @brief Construct a new MHDR object
     *
     */
    MHDR() {}

    /**
     * @brief Destroy the MHDR object
     *
     */
    virtual ~MHDR() = default;

    /**
     * @brief Get the Message Length object
     *
     * @return constexpr std::uint32_t
     */
    constexpr std::uint32_t getMessageLength() const { return message_length; }

    /**
     * @brief Get the Message Type object
     *
     * @return MessageType
     */
    constexpr MessageType getMessageType() const { return message_type; }

    /**
     * @brief Set the Message Length object
     *
     * @param message_length
     */
    void setMessageLength(const std::uint32_t message_length) {
        this->message_length = message_length;
    }

    /**
     * @brief Set the Message Type object
     *
     * @param message_type
     */

    void setMessageType(const MessageType message_type) {
        this->message_type = message_type;
    }

  protected:
  private:
    std::uint32_t message_length;
    MessageType message_type;
};

template <> inline const std::vector<std::byte> serialize(const MHDR &mhdr) {
    std::vector<std::byte> result{};

    const auto length = serialize(mhdr.getMessageLength());
    const auto type = serialize(mhdr.getMessageType());

    std::move(length.cbegin(), length.cend(), std::back_inserter(result));
    std::move(type.cbegin(), type.cend(), std::back_inserter(result));

    return result;
}

template <> inline const MHDR deserialize(const std::vector<std::byte> &bytes) {
    MHDR mhdr{};

    mhdr.setMessageLength(deserialize<std::uint32_t>(
        std::vector<std::byte>{bytes.cbegin(), bytes.cbegin() + 4}));
    mhdr.setMessageType(deserialize<MessageType>(
        std::vector<std::byte>{bytes.cbegin() + 4, bytes.cbegin() + 8}));

    return mhdr;
}

} // namespace cisco::common

#endif