#pragma once

#ifndef _CTM_CISCO_COMMON_MHDR_H_
#define _CTM_CISCO_COMMON_MHDR_H_

#include <cstdint>

#include "./serializable.hpp"
#include "./message_type.hpp"

namespace cisco::common {

/**
 * @brief Message Header
 *
 */
class MHDR : public Serializable<MHDR> {
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

  /**
   * @brief 메시지 헤더 패킷 직렬화
   *
   * @return const std::vector<std::byte>
   */
  const std::vector<std::byte> serialize() const override;

  /**
   * @brief 메시지 헤더 패킷 역직렬화
   *
   * @param bytes
   * @return const MHDR
   */
  void deserialize(const std::vector<std::byte> &bytes) override;

protected:
private:
  std::uint32_t message_length;
  MessageType message_type;
};

} // namespace cisco::common

#endif