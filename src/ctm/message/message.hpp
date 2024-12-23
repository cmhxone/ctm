#pragma once

#ifndef _CTM_CTM_MESSAGE_MESSAGE_HPP_
#define _CTM_CTM_MESSAGE_MESSAGE_HPP_

#include <cstddef>
#include <vector>

namespace ctm::message {
/**
 * @brief CTI ↔ CTM ↔ Client 메시지 클래스
 *
 */
class Message {
public:
  /**
   * @brief 메시지 유형
   *
   */
  enum MessageType {
    AGENT_MESSAGE,
    STATE_REQUEST_MESSAGE,
  };

  /**
   * @brief Construct a new Message object
   *
   */
  Message() {};
  /**
   * @brief Destroy the Message object
   *
   */
  virtual ~Message() = default;

  /**
   * @brief 메시지 패킹
   *
   * @return const std::vector<std::byte>
   */
  virtual const std::vector<std::byte> pack() const = 0;
  /**
   * @brief 메시지 언패킹
   *
   * @param packed_message
   */
  virtual void unpack(const std::vector<std::byte> &packed_message) = 0;

  /**
   * @brief Get the Type object
   *
   * @return constexpr MessageType
   */
  virtual constexpr MessageType getType() const = 0;

protected:
private:
};
} // namespace ctm::message

#endif