#pragma once

#ifndef _CTM_CISCO_CONTROL_QUERY_AGENT_STATE_CONF_HPP_
#define _CTM_CISCO_CONTROL_QUERY_AGENT_STATE_CONF_HPP_

#include "../common/floating_data.hpp"
#include "../common/mhdr.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cisco::control {
class QueryAgentStateConf {
public:
  /**
   * @brief Construct a new Query Agent State Conf object
   *
   */
  QueryAgentStateConf() {}
  /**
   * @brief Destroy the Query Agent State Conf object
   *
   */
  virtual ~QueryAgentStateConf() = default;

  /**
   * @brief Get the MHDR object
   *
   * @return const common::MHDR
   */
  const common::MHDR getMHDR() const { return mhdr; }
  /**
   * @brief Get the Invoke ID object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getInvokeID() const { return invoke_id; }
  /**
   * @brief Get the Agent State object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getAgentState() const { return agent_state; }
  /**
   * @brief Get the Num Skill Groups object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getNumSkillGroups() const { return num_skill_groups; }
  /**
   * @brief Get the MRD ID object
   *
   * @return constexpr std::int32_t
   */
  constexpr std::int32_t getMRDID() const { return mrd_id; }
  /**
   * @brief Get the Agent Availability Status object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint32_t getAgentAvailabilityStatus() const {
    return agent_availability_status;
  }
  /**
   * @brief Get the Num Tasks object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getNumTasks() const { return num_tasks; }
  /**
   * @brief Get the Agent Mode object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getAgentMode() const { return agent_mode; }
  /**
   * @brief Get the Max Task Limit object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getMaxTaskLimit() const { return max_task_limit; }
  /**
   * @brief Get the ICM Agent ID object
   *
   * @return constexpr std::int32_t
   */
  constexpr std::int32_t getICMAgentID() const { return icm_agent_id; }
  /**
   * @brief Get the Department ID object
   *
   * @return constexpr std::int32_t
   */
  constexpr std::int32_t getDepartmentID() const { return department_id; }
  /**
   * @brief Get the Agent ID object
   *
   * @return const std::string
   */
  const std::string getAgentID() const { return agent_id.value_or(""); }
  /**
   * @brief Get the Agent Extension object
   *
   * @return const std::string
   */
  const std::string getAgentExtension() const {
    return agent_extension.value_or("");
  }
  /**
   * @brief Get the Agent Instrument object
   *
   * @return const std::string
   */
  const std::string getAgentInstrument() const {
    return agent_instrument.value_or("");
  }
  /**
   * @brief Get the Skill Group Number object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getSkillGroupNumber() const {
    return skill_group_number.value_or(0);
  }
  /**
   * @brief Get the Skill Group ID object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getSkillGroupID() const {
    return skill_group_id.value_or(0);
  }
  /**
   * @brief Get the Skill Group Priority object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getSkillGroupPriority() const {
    return skill_group_priority.value_or(0);
  }
  /**
   * @brief Get the Skill Group State object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getSkillGroupState() const {
    return skill_group_state.value_or(0);
  }
  /**
   * @brief Get the Internal Agent State object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getInternalAgentState() const {
    return internal_agent_state.value_or(0);
  }
  /**
   * @brief Get the Max Beyond Task Limit object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getMaxBeyondTaskLimit() const {
    return max_beyond_task_limit.value_or(0);
  }

  /**
   * @brief Set the MHDR object
   *
   * @param mhdr
   */
  void setMHDR(const common::MHDR &mhdr) { this->mhdr = mhdr; }
  /**
   * @brief Set the Invoke ID object
   *
   * @param invoke_id
   */
  void setInvokeID(const std::uint32_t invoke_id) {
    this->invoke_id = invoke_id;
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
   * @brief Set the Num Skill Groups object
   *
   * @param num_skill_groups
   */
  void setNumSkillGroups(const std::uint16_t num_skill_groups) {
    this->num_skill_groups = num_skill_groups;
  }
  /**
   * @brief Set the MRD ID object
   *
   * @param mrd_id
   */
  void setMRDID(const std::int32_t mrd_id) { this->mrd_id = mrd_id; }
  /**
   * @brief Set the Agent Availability Status object
   *
   * @param agent_availability_status
   */
  void
  setAgentAvailabilityStatus(const std::uint32_t agent_availability_status) {
    this->agent_availability_status = agent_availability_status;
  }
  /**
   * @brief Set the Num Tasks object
   *
   * @param num_tasks
   */
  void setNumTasks(const std::uint32_t num_tasks) {
    this->num_tasks = num_tasks;
  }
  /**
   * @brief Set the Agent Mode object
   *
   * @param agent_mode
   */
  void setAgentMode(const std::uint16_t agent_mode) {
    this->agent_mode = agent_mode;
  }
  /**
   * @brief Set the Max Task Limit object
   *
   * @param max_task_limit
   */
  void setMaxTaskLimit(const std::uint32_t max_task_limit) {
    this->max_task_limit = max_task_limit;
  }
  /**
   * @brief Set the ICM Agent ID object
   *
   * @param icm_agent_id
   */
  void setICMAgentID(const std::int32_t icm_agent_id) {
    this->icm_agent_id = icm_agent_id;
  }
  /**
   * @brief Set the Department ID object
   *
   * @param department_id
   */
  void setDepartmentID(const std::int32_t department_id) {
    this->department_id = department_id;
  }
  /**
   * @brief Set the Agent ID object
   *
   * @param agent_id
   */
  void setAgentID(const std::string_view &agent_id) {
    this->agent_id = agent_id.data();
  }
  /**
   * @brief Set the Agent Extension object
   *
   * @param agent_extension
   */
  void setAgentExtension(const std::string_view &agent_extension) {
    this->agent_extension = agent_extension.data();
  }
  /**
   * @brief Set the Agent Instrument object
   *
   * @param agent_instrument
   */
  void setAgentInstrument(const std::string_view &agent_instrument) {
    this->agent_instrument = agent_instrument.data();
  }
  /**
   * @brief Set the Skill Group Number object
   *
   * @param skill_group_number
   */
  void setSkillGroupNumber(const std::uint32_t skill_group_number) {
    this->skill_group_number = skill_group_number;
  }
  /**
   * @brief Set the Skill Group ID object
   *
   * @param skill_group_id
   */
  void setSkillGroupID(const std::uint32_t skill_group_id) {
    this->skill_group_id = skill_group_id;
  }
  /**
   * @brief Set the Skill Group Priority object
   *
   * @param skill_group_priority
   */
  void setSkillGroupPriority(const std::uint16_t skill_group_priority) {
    this->skill_group_priority = skill_group_priority;
  }
  /**
   * @brief Set the Skill Group State object
   *
   * @param skill_group_state
   */
  void setSkillGroupState(const std::uint16_t skill_group_state) {
    this->skill_group_state = skill_group_state;
  }
  /**
   * @brief Set the Internal Agent State object
   *
   * @param internal_agent_state
   */
  void setInternalAgentState(const std::uint16_t internal_agent_state) {
    this->internal_agent_state = internal_agent_state;
  }
  /**
   * @brief Set the Max Beyond Task Limit object
   *
   * @param max_beyond_task_limit
   */
  void setMaxBeyondTaskLimit(const std::uint32_t max_beyond_task_limit) {
    this->max_beyond_task_limit = max_beyond_task_limit;
  }

protected:
private:
  // 고정영역
  common::MHDR mhdr;
  std::uint32_t invoke_id;
  std::uint16_t agent_state;
  std::uint16_t num_skill_groups;
  std::int32_t mrd_id;
  std::uint32_t agent_availability_status;
  std::uint32_t num_tasks;
  std::uint16_t agent_mode;
  std::uint32_t max_task_limit;
  std::int32_t icm_agent_id;
  std::int32_t department_id;
  // 가변 영역
  std::optional<std::string> agent_id;
  std::optional<std::string> agent_extension;
  std::optional<std::string> agent_instrument;
  std::optional<std::uint32_t> skill_group_number;
  std::optional<std::uint32_t> skill_group_id;
  std::optional<std::uint16_t> skill_group_priority;
  std::optional<std::uint16_t> skill_group_state;
  std::optional<std::uint16_t> internal_agent_state;
  std::optional<std::uint32_t> max_beyond_task_limit;
};
} // namespace cisco::control

template <>
inline const cisco::control::QueryAgentStateConf
cisco::common::deserialize(const std::vector<std::byte> &bytes) {
  cisco::control::QueryAgentStateConf query_agent_state_conf;

  size_t packet_index = 0;
  size_t field_size = 0;

  field_size = 8;
  query_agent_state_conf.setMHDR(deserialize<common::MHDR>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  field_size = 4;
  query_agent_state_conf.setInvokeID(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  field_size = 2;
  query_agent_state_conf.setAgentState(deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  query_agent_state_conf.setNumSkillGroups(deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  field_size = 4;
  query_agent_state_conf.setMRDID(deserialize<std::int32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  query_agent_state_conf.setAgentAvailabilityStatus(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  field_size = 4;
  query_agent_state_conf.setNumTasks(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  field_size = 2;
  query_agent_state_conf.setAgentMode(deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  field_size = 4;
  query_agent_state_conf.setMaxTaskLimit(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  query_agent_state_conf.setICMAgentID(deserialize<std::int32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  query_agent_state_conf.setDepartmentID(deserialize<std::int32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  // 가변 데이터 파싱
  while (packet_index < query_agent_state_conf.getMHDR().getMessageLength()) {
    const cisco::common::FloatingData floating_data =
        cisco::common::deserialize<cisco::common::FloatingData>(
            std::vector<std::byte>{bytes.cbegin() + packet_index,
                                   bytes.cend()});
    packet_index += 4 + floating_data.getData().size();

    switch (floating_data.getTag()) {
    case cisco::common::TagValue::AGENT_ID_TAG: {
      std::string agent_id;
      agent_id.resize(floating_data.getData().size());
      std::memcpy(agent_id.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      query_agent_state_conf.setAgentID(agent_id);
    } break;
    case cisco::common::TagValue::AGENT_EXTENSION_TAG: {
      std::string agent_extension;
      agent_extension.resize(floating_data.getData().size());
      std::memcpy(agent_extension.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      query_agent_state_conf.setAgentExtension(agent_extension);
    } break;
    case cisco::common::TagValue::AGENT_INSTRUMENT_TAG: {
      std::string agent_instrument;
      agent_instrument.resize(floating_data.getData().size());
      std::memcpy(agent_instrument.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      query_agent_state_conf.setAgentInstrument(agent_instrument);
    } break;
    case cisco::common::TagValue::SKILL_GROUP_NUMBER_TAG: {
      query_agent_state_conf.setSkillGroupNumber(
          deserialize<std::uint32_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::SKILL_GROUP_ID_TAG: {
      query_agent_state_conf.setSkillGroupID(
          deserialize<std::uint32_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::SKILL_GROUP_PRIORITY_TAG: {
      query_agent_state_conf.setSkillGroupPriority(
          deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::SKILL_GROUP_STATE_TAG: {
      query_agent_state_conf.setSkillGroupState(
          deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::INTERNAL_AGENT_STATE_TAG: {
      query_agent_state_conf.setInternalAgentState(
          deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::MAX_BEYOND_TASK_LIMIT_TAG: {
      query_agent_state_conf.setMaxBeyondTaskLimit(
          deserialize<std::uint32_t>(floating_data.getData()));
    } break;
    default:
      break;
    }
  }

  return query_agent_state_conf;
}

#endif