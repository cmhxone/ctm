#pragma once

#ifndef _CTM_CTM_AGENT_INFO_HPP_
#define _CTM_CTM_AGENT_INFO_HPP_

#include "../channel/event/bridge_event.hpp"
#include "../channel/event_channel.hpp"

#include <msgpack.hpp>
#include <spdlog/spdlog.h>

#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>

namespace ctm {
/**
 * @brief 상담원 상태
 *
 */
class AgentInfo {
public:
  /**
   * @brief Construct a new Agent Info object
   *
   */
  AgentInfo() {}
  /**
   * @brief Destroy the Agent Info object
   *
   */
  virtual ~AgentInfo() = default;

  /**
   * @brief 비교 연산 오버로딩
   *
   * @param rhs
   * @return true
   * @return false
   */
  constexpr bool operator==(const AgentInfo &rhs) const {
    return getAgentID() == rhs.getAgentID();
  }

  /**
   * @brief Get the ICM Agent ID object
   *
   * @return constexpr std::int32_t
   */
  constexpr std::int32_t getICMAgentID() const { return icm_agent_id; }
  /**
   * @brief Get the Agent I D object
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
  constexpr std::uint64_t getStateDuration() const { return state_duration; }
  /**
   * @brief Get the Reason Code object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getReasonCode() const { return reason_code; }
  /**
   * @brief Get the Skill Group I D object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getSkillGroupID() const { return skill_group_id; }
  /**
   * @brief Get the Direction object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getDirection() const { return direction; }
  /**
   * @brief Get the Extension object
   *
   * @return const std::string
   */
  const std::string getExtension() const { return extension; }

  /**
   * @brief Set the ICM Agent ID object
   *
   * @param icm_agent_id
   */
  void setICMAgentID(const std::int32_t icm_agent_id) {
    this->icm_agent_id = icm_agent_id;
  }
  /**
   * @brief Set the Agent I D object
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
    // 상태 지속시간은 UNIX epoch time으로 변환해서 저장한다
    // 클라이언트에서는 상태 시작시간으로 판단하면 된다
    std::uint64_t now_epoch =
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();

    this->state_duration =
        now_epoch - static_cast<std::uint64_t>(state_duration);
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
   * @brief Set the Skill Group I D object
   *
   * @param skill_group_id
   */
  void setSkillGroupID(const std::uint16_t skill_group_id) {
    this->skill_group_id = skill_group_id;
  }
  /**
   * @brief Set the Direction object
   *
   * @param direction
   */
  void setDirection(const std::uint32_t direction) {
    this->direction = direction;
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
   * @brief 데이터 변경 내용을 클라이언트에게 브로드 캐스팅 하는 메소드
   *
   */
  void broadcast() {
    channel::EventChannel<channel::event::BridgeEvent>::getInstance()->publish(
        channel::event::BridgeEvent{
            channel::event::BridgeEvent::BridgeEventDestination::CLIENT,
            channel::event::BridgeEvent::BridgeEventMessage{
                .type = channel::event::BridgeEvent::BridgeEventType::
                    BROADCAST_AGENT_STATE,
                .message = pack()}});

    spdlog::debug(
        "icm_agent_id: {}, agent_id: {}, agent_state: {}, state_duration: {}, "
        "reason_code: {}, skill_group_id: {}, direction: {}, extension: {}",
        getICMAgentID(), getAgentID(), getAgentState(), getStateDuration(),
        getReasonCode(), getSkillGroupID(), getDirection(), getExtension());
  }

  /**
   * @brief 메시지 패킹
   *
   * @return const std::vector<std::byte>
   */
  const std::vector<std::byte> pack() const {
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
  void unpack(const std::vector<std::byte> &packed_message) {
    msgpack::object_handle obj_handle = msgpack::unpack(
        (const char *)packed_message.data(), packed_message.size());
    msgpack::object obj = obj_handle.get();
    obj.convert(*this);
  }

  MSGPACK_DEFINE(icm_agent_id, agent_id, agent_state, state_duration,
                 reason_code, skill_group_id, direction, extension);

protected:
private:
  std::int32_t icm_agent_id{0};
  std::string agent_id{""};
  std::uint16_t agent_state{0};
  std::uint64_t state_duration{static_cast<std::uint64_t>(
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count())};
  std::uint16_t reason_code{0};
  std::uint16_t skill_group_id{0};
  std::uint32_t direction{0};
  std::string extension{""};
};
} // namespace ctm

#endif