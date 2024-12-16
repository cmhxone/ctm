#pragma once

#ifndef _CTM_CTM_AGENT_INFO_HPP_
#define _CTM_CTM_AGENT_INFO_HPP_

#include <cstdint>
#include <string>
#include <string_view>

namespace ctm {
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
   * @brief
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
  constexpr std::uint16_t getStateDuration() const { return state_duration; }
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
   * @brief Set the Skill Group I D object
   *
   * @param skill_group_id
   */
  void setSkillGroupID(const std::uint16_t skill_group_id) {
    this->skill_group_id = skill_group_id;
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
    // TODO: 업데이트 이후 본 데이터를 클라이언트에게 브로드캐스팅한다
  }

protected:
private:
  std::int32_t icm_agent_id;
  std::string agent_id;
  std::uint16_t agent_state;
  std::uint16_t state_duration;
  std::uint16_t reason_code;
  std::uint16_t skill_group_id;
  std::string extension;
};
} // namespace ctm

#endif