#pragma once

#ifndef _CTM_CTM_MESSAGE_STATE_REQUEST_MESSAGE_HPP_
#define _CTM_CTM_MESSAGE_STATE_REQUEST_MESSAGE_HPP_

#include "./message.hpp"

#include <msgpack.hpp>

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace ctm::message {
/**
 * @brief 상담원 상태 요청 메시지
 *
 */
class StateRequestMessage : public Message {
public:
  /**
   * @brief Construct a new State Request Message object
   *
   */
  StateRequestMessage() {}
  /**
   * @brief Destroy the State Request Message object
   *
   */
  virtual ~StateRequestMessage() = default;

  /**
   * @brief Get the Agent List object
   *
   * @return const std::vector<std::string>&
   */
  const std::vector<std::string> &getAgentList() const { return agent_list; }
  /**
   * @brief 상담원 목록 추가
   *
   * @param agent
   * @param delimiter
   */
  void addAgent(const std::string_view agent, const char delimiter = ',') {
    auto spos = agent.cbegin();
    auto epos = agent.cend();

    while (spos != agent.cend()) {
      const auto npos = std::find(spos, agent.cend(), delimiter);
      agent_list.emplace_back(spos, npos);
      spos = npos != agent.cend() ? npos + 1 : npos;
    }
  }

  /**
   * @brief 메시지 패킹
   *
   * @return const std::vector<std::byte>
   */
  virtual const std::vector<std::byte> pack() const override {
    std::ostringstream stream{};
    msgpack::pack(stream, *this);

    std::vector<std::byte> buffer{};
    for (const char ch : stream.str()) {
      buffer.emplace_back(static_cast<std::byte>(ch));
    }

    return buffer;
  }

  /**
   * @brief 메시지 언패킹
   *
   * @param packed_message
   */
  virtual void unpack(const std::vector<std::byte> &packed_message) override {
    msgpack::object_handle obj_handle = msgpack::unpack(
        (const char *)packed_message.data(), packed_message.size());
    msgpack::object obj = obj_handle.get();
    obj.convert(*this);
  }

  /**
   * @brief Get the Type object
   *
   * @return constexpr MessageType
   */
  virtual constexpr MessageType getType() const override {
    return MessageType::STATE_REQUEST_MESSAGE;
  }

  MSGPACK_DEFINE(agent_list);

protected:
private:
  std::vector<std::string> agent_list;
};
} // namespace ctm::message

#endif