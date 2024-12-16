#pragma once

#ifndef _CTM_CTM_MESSAGE_MESSAGE_HPP_
#define _CTM_CTM_MESSAGE_MESSAGE_HPP_

#include <string>
#include <string_view>

namespace ctm::message {
/**
 * @brief CTI ↔ CTM ↔ Client 메시지 클래스
 *
 */
class Message {
public:
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
   * @return const std::string
   */
  virtual const std::string pack() const = 0;
  /**
   * @brief 메시지 언패킹
   *
   * @param packed_message
   */
  virtual void unpack(const std::string_view packed_message) = 0;

protected:
private:
};
} // namespace ctm::message

#endif