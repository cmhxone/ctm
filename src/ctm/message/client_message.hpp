#pragma once

#ifndef _CTM_CTM_MESSAGE_CLIENT_MESSAGE_HPP_
#define _CTM_CTM_MESSAGE_CLIENT_MESSAGE_HPP_

#include "./message.hpp"

#include <msgpack/v3/object_decl.hpp>
#include <msgpack/v3/object_fwd_decl.hpp>
#include <msgpack/v3/pack_decl.hpp>
#include <msgpack/v3/unpack.hpp>

#include <cstdint>
#include <sstream>
#include <string>
#include <string_view>

namespace ctm::message {
class ClientMessage : public Message {
public:
  /**
   * @brief Construct a new Client Message object
   *
   */
  ClientMessage() {}
  /**
   * @brief Destroy the Client Message object
   *
   */
  virtual ~ClientMessage() = default;

  /**
   * @brief Get the Agent ID object
   *
   * @return const std::string
   */
  const std::string getAgentID() const { return agent_id; }
  /**
   * @brief Get the Agent State object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getAgentState() const { return agent_state; }
  /**
   * @brief Get the State Duration object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getStateDuration() const { return state_duration; }
  /**
   * @brief Get the Reason Code object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getReasonCode() const { return reason_code; }
  /**
   * @brief Get the Extension object
   *
   * @return const std::string
   */
  const std::string getExtension() const { return extension; }

  /**
   * @brief Set the Agent ID object
   *
   * @param agent_id
   */
  void setAgentID(const std::string_view agent_id) {
    this->agent_id = agent_id.data();
  }
  /**
   * @brief Set the Agent State object
   *
   * @param agent_state
   */
  void setAgentState(const std::uint16_t agent_state) {
    this->agent_state = agent_state;
  }
  /**
   * @brief Set the State Duration object
   *
   * @param state_duration
   */
  void setStateDuration(const std::uint16_t state_duration) {
    this->state_duration = state_duration;
  }
  /**
   * @brief Set the Reason Code object
   *
   * @param reason_code
   */
  void setReasonCode(const std::uint16_t reason_code) {
    this->reason_code = reason_code;
  }
  /**
   * @brief Set the Extension object
   *
   * @param extension
   */
  void setExtension(const std::string_view extension) {
    this->extension = extension.data();
  }

  /**
   * @brief 메시지 패킹
   *
   * @return const std::string
   */
  virtual const std::string pack() const override {
    std::stringstream stream;
    msgpack::pack(stream, this);

    return stream.str();
  }

  /**
   * @brief 메시지 언패킹
   *
   * @param packed_message
   */
  virtual void unpack(const std::string_view packed_message) override {
    msgpack::object_handle obj_handle =
        msgpack::unpack(packed_message.data(), packed_message.length());
    msgpack::object obj = obj_handle.get();
    obj.convert(*this);
  }

protected:
private:
  std::string agent_id;
  std::uint16_t agent_state;
  std::uint16_t state_duration;
  std::uint16_t reason_code;
  std::string extension;
};
} // namespace ctm::message

#endif