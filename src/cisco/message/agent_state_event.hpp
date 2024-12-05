#pragma once

#ifndef _CTM_CISCO_MESSAGE_AGENT_STATE_EVENT_HPP_
#define _CTM_CISCO_MESSAGE_AGENT_STATE_EVENT_HPP_

#include "../common/floating_data.hpp"
#include "../common/mhdr.hpp"
#include "../common/serializable.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cisco::message {
class AgentStateEvent {
public:
  /**
   * @brief Construct a new Agent State Event object
   *
   */
  AgentStateEvent() {}

  /**
   * @brief Destroy the Agent State Event object
   *
   */
  virtual ~AgentStateEvent() = default;

  const cisco::common::MHDR getMHDR() const { return mhdr; }
  constexpr std::uint32_t getMonitorID() const { return monitor_id; }
  constexpr std::uint32_t getPeripheralID() const { return peripheral_id; }
  constexpr std::uint32_t getSessionID() const { return session_id; }
  constexpr std::uint16_t getPeripheralType() const { return peripheral_type; }
  constexpr std::uint16_t getSkillGroupState() const {
    return skill_group_state;
  }
  constexpr std::uint32_t getStateDuration() const { return state_duration; }
  constexpr std::uint32_t getSkillGroupNumber() const {
    return skill_group_number;
  }
  constexpr std::uint32_t getSkillGroupID() const { return skill_group_id; }
  constexpr std::uint16_t getSkillGroupPriority() const {
    return skill_group_priority;
  }
  constexpr std::uint16_t getAgentState() const { return agent_state; }
  constexpr std::uint16_t getEventReasonCode() const {
    return event_reason_code;
  }
  constexpr std::int32_t getMRDID() const { return mrd_id; }
  constexpr std::uint32_t getNumTask() const { return num_task; }
  constexpr std::uint16_t getAgentMode() const { return agent_mode; }
  constexpr std::uint32_t getMaxTaskLimit() const { return max_task_limit; }
  constexpr std::int32_t getICMAgentID() const { return icm_agent_id; }
  constexpr std::uint32_t getAgentAvailabilityStatus() const {
    return agent_availability_status;
  }
  constexpr std::uint16_t getNumFltSkillGroups() const {
    return num_flt_skill_groups;
  }
  constexpr std::int32_t getDepartmentID() const { return department_id; }
  /**
   * @brief Get the CTI Client Signature object
   *
   * @return const std::string
   */
  const std::string getCTIClientSignature() const {
    return this->cti_client_signature->c_str();
  }
  /**
   * @brief Get the Agent ID object
   *
   * @return const std::string
   */
  const std::string getAgentID() const { return this->agent_id->data(); }
  /**
   * @brief Get the Agent Extension object
   *
   * @return const std::string
   */
  const std::string getAgentExtension() const {
    return this->agent_extension->data();
  }
  /**
   * @brief Get the Active Terminal object
   *
   * @return const std::string
   */
  const std::string getActiveTerminal() const {
    return this->active_terminal->data();
  }
  /**
   * @brief Get the Agent Instrument object
   *
   * @return const std::string
   */
  const std::string getAgentInstrument() const {
    return this->agent_instrument->data();
  }
  /**
   * @brief Get the Duration object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getDuration() const {
    return this->duration.value_or(0);
  }
  /**
   * @brief Get the Next Agent State object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getNextAgentState() const {
    return this->next_agent_state.value_or(0);
  }
  /**
   * @brief Get the Direction object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint32_t getDirection() const {
    return this->direction.value_or(0);
  }
  constexpr std::int32_t getFltSkillGroupNumber() const {
    return this->flt_skill_group_number.value_or(0);
  }
  /**
   * @brief Get the Flt Skill Group ID object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getFltSkillGroupID() const {
    return this->flt_skill_group_id.value_or(0);
  }
  /**
   * @brief Get the Flt Skill Group Priority object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getFltSkillGroupPriority() const {
    return this->flt_skill_group_priority.value_or(0);
  }
  /**
   * @brief Get the Flt Skill Group State object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getFltSkillGroupState() const {
    return this->flt_skill_group_state.value_or(0);
  }
  /**
   * @brief Get the Max Beyond Task Limit object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getMaxBeyondTaskLimit() const {
    return this->max_beyond_task_limit.value_or(0);
  }

  /**
   * @brief Set the MHDR object
   *
   * @param mhdr
   */
  void setMHDR(const cisco::common::MHDR &mhdr) { this->mhdr = mhdr; }
  /**
   * @brief Set the Monitor ID object
   *
   * @param monitor_id
   */
  void setMonitorID(const std::uint32_t monitor_id) {
    this->monitor_id = monitor_id;
  }
  /**
   * @brief Set the Peripheral ID object
   *
   * @param peripheral_id
   */
  void setPeripheralID(const std::uint32_t peripheral_id) {
    this->peripheral_id = peripheral_id;
  }
  /**
   * @brief Set the Session ID object
   *
   * @param session_id
   */
  void setSessionID(const std::uint32_t session_id) {
    this->session_id = session_id;
  }
  /**
   * @brief Set the Peripheral Type object
   *
   * @param peripheral_type
   */
  void setPeripheralType(const std::uint16_t peripheral_type) {
    this->peripheral_type = peripheral_type;
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
   * @brief Set the State Duration object
   *
   * @param state_duration
   */
  void setStateDuration(const std::uint32_t state_duration) {
    this->state_duration = state_duration;
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
   * @brief Set the Skill Group I D object
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
   * @brief Set the Agent State object
   *
   * @param agent_state
   */
  void setAgentState(const std::uint16_t agent_state) {
    this->agent_state = agent_state;
  }
  /**
   * @brief Set the Event Reason Code object
   *
   * @param event_reason_code
   */
  void setEventReasonCode(const std::uint16_t event_reason_code) {
    this->event_reason_code = event_reason_code;
  }
  /**
   * @brief Set the MRD ID object
   *
   * @param mrd_id
   */
  void setMRDID(const std::int32_t mrd_id) { this->mrd_id = mrd_id; }
  /**
   * @brief Set the Num Task object
   *
   * @param num_task
   */
  void setNumTask(const std::uint32_t num_task) { this->num_task = num_task; }
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
   * @brief Set the Agent Availability Status object
   *
   * @param agent_availability_status
   */
  void
  setAgentAvailabilityStatus(const std::uint32_t agent_availability_status) {
    this->agent_availability_status = agent_availability_status;
  }
  /**
   * @brief Set the Num Flt Skill Groups object
   *
   * @param num_flt_skill_groups
   */
  void setNumFltSkillGroups(const std::uint16_t num_flt_skill_groups) {
    this->num_flt_skill_groups = num_flt_skill_groups;
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
   * @brief Set the CTI Client Signature object
   *
   * @param cti_client_signature
   */
  void setCTIClientSignature(const std::string_view &cti_client_signature) {
    this->cti_client_signature = cti_client_signature.data();
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
   * @brief Set the Active Terminal object
   *
   * @param active_terminal
   */
  void setActiveTerminal(const std::string_view &active_terminal) {
    this->active_terminal = active_terminal.data();
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
   * @brief Set the Duration object
   *
   * @param duration
   */
  void setDuration(const std::uint32_t duration) { this->duration = duration; }
  /**
   * @brief Set the Next Agent State object
   *
   * @param next_agent_state
   */
  void setNextAgentState(const std::uint16_t next_agent_state) {
    this->next_agent_state = next_agent_state;
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
   * @brief Set the Flt Skill Group Number object
   *
   * @param flt_skill_group_number
   */
  void setFltSkillGroupNumber(const std::int32_t flt_skill_group_number) {
    this->flt_skill_group_number = flt_skill_group_number;
  }
  /**
   * @brief Set the Flt Skill Group ID object
   *
   * @param flt_skill_group_id
   */
  void setFltSkillGroupID(const std::uint32_t flt_skill_group_id) {
    this->flt_skill_group_id = flt_skill_group_id;
  }
  /**
   * @brief Set the Flt Skill Group Priority object
   *
   * @param flt_skill_group_priority
   */
  void setFltSkillGroupPriority(const std::uint16_t flt_skill_group_priority) {
    this->flt_skill_group_priority = flt_skill_group_priority;
  }
  /**
   * @brief Set the Flt Skill Group State object
   *
   * @param flt_skill_group_state
   */
  void setFltSkillGroupState(const std::uint16_t flt_skill_group_state) {
    this->flt_skill_group_state = flt_skill_group_state;
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
  // 고정 영역 데이터
  common::MHDR mhdr;
  std::uint32_t monitor_id;
  std::uint32_t peripheral_id;
  std::uint32_t session_id;
  std::uint16_t peripheral_type;
  std::uint16_t skill_group_state;
  std::uint32_t state_duration;
  std::uint32_t skill_group_number;
  std::uint32_t skill_group_id;
  std::uint16_t skill_group_priority;
  std::uint16_t agent_state;
  std::uint16_t event_reason_code;
  std::int32_t mrd_id;
  std::uint32_t num_task;
  std::uint16_t agent_mode;
  std::uint32_t max_task_limit;
  std::int32_t icm_agent_id;
  std::uint32_t agent_availability_status;
  std::uint16_t num_flt_skill_groups;
  std::int32_t department_id;
  // 가변 영역 데이터
  std::optional<std::string> cti_client_signature;
  std::optional<std::string> agent_id;
  std::optional<std::string> agent_extension;
  std::optional<std::string> active_terminal;
  std::optional<std::string> agent_instrument;
  std::optional<std::uint32_t> duration;
  std::optional<std::uint16_t> next_agent_state;
  std::optional<std::uint32_t> direction;
  std::optional<std::int32_t> flt_skill_group_number;
  std::optional<std::uint32_t> flt_skill_group_id;
  std::optional<std::uint16_t> flt_skill_group_priority;
  std::optional<std::uint16_t> flt_skill_group_state;
  std::optional<std::uint32_t> max_beyond_task_limit;
};
} // namespace cisco::message

template <>
inline const cisco::message::AgentStateEvent
cisco::common::deserialize(const std::vector<std::byte> &bytes) {
  cisco::message::AgentStateEvent result{};

  size_t packet_index = 0;
  size_t field_size = 0;

  // 고정 영역
  field_size = 8;
  result.setMHDR(cisco::common::deserialize<cisco::common::MHDR>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 4;
  result.setMonitorID(cisco::common::deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  result.setPeripheralID(cisco::common::deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  result.setSessionID(cisco::common::deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 2;
  result.setPeripheralType(cisco::common::deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  result.setSkillGroupState(cisco::common::deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 4;
  result.setStateDuration(cisco::common::deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  result.setSkillGroupNumber(cisco::common::deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  result.setSkillGroupID(cisco::common::deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 2;
  result.setSkillGroupPriority(cisco::common::deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 2;
  result.setAgentState(cisco::common::deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  result.setEventReasonCode(cisco::common::deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 4;
  result.setMRDID(cisco::common::deserialize<std::int32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  result.setNumTask(cisco::common::deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 2;
  result.setAgentMode(cisco::common::deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 4;
  result.setMaxTaskLimit(cisco::common::deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  result.setICMAgentID(cisco::common::deserialize<std::int32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  result.setAgentAvailabilityStatus(cisco::common::deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 2;
  result.setNumFltSkillGroups(cisco::common::deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;
  field_size = 4;
  result.setDepartmentID(cisco::common::deserialize<std::int32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index, bytes.cend()}));
  packet_index += field_size;

  // 가변 영역
  while (packet_index < result.getMHDR().getMessageLength()) {
    const cisco::common::FloatingData floating_data =
        cisco::common::deserialize<cisco::common::FloatingData>(
            std::vector<std::byte>{bytes.cbegin() + packet_index,
                                   bytes.cend()});
    packet_index += 4 + floating_data.getData().size();

    switch (floating_data.getTag()) {
    case cisco::common::TagValue::CTI_CLIENT_SIGNATURE_TAG: {
      std::string cti_client_signature;
      cti_client_signature.resize(floating_data.getData().size());
      std::memcpy(cti_client_signature.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      result.setCTIClientSignature(cti_client_signature);
    } break;
    case cisco::common::TagValue::AGENT_ID_TAG: {
      std::string agent_id;
      agent_id.resize(floating_data.getData().size());
      std::memcpy(agent_id.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      result.setAgentID(agent_id);
    } break;
    case cisco::common::TagValue::AGENT_EXTENSION_TAG: {
      std::string agent_extension;
      agent_extension.resize(floating_data.getData().size());
      std::memcpy(agent_extension.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      result.setAgentExtension(agent_extension);
    } break;
    case cisco::common::TagValue::FLT_TERM_DEVICE_NAME: {
      std::string active_terminal;
      active_terminal.resize(floating_data.getData().size());
      std::memcpy(active_terminal.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      result.setActiveTerminal(active_terminal);
    } break;
    case cisco::common::TagValue::AGENT_INSTRUMENT_TAG: {
      std::string agent_instrument;
      agent_instrument.resize(floating_data.getData().size());
      std::memcpy(agent_instrument.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      result.setCTIClientSignature(agent_instrument);
    } break;
    case cisco::common::TagValue::DURATION_TAG: {
      result.setDuration(
          cisco::common::deserialize<std::uint32_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::EXT_AGENT_STATE_TAG: {
      result.setAgentState(
          cisco::common::deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::DIRECTION_TAG: {
      result.setDirection(
          cisco::common::deserialize<std::uint32_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::SKILL_GROUP_NUMBER_TAG: {
      result.setFltSkillGroupNumber(
          cisco::common::deserialize<std::int32_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::SKILL_GROUP_ID_TAG: {
      result.setFltSkillGroupID(
          cisco::common::deserialize<std::uint32_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::SKILL_GROUP_PRIORITY_TAG: {
      result.setFltSkillGroupPriority(
          cisco::common::deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::SKILL_GROUP_STATE_TAG: {
      result.setFltSkillGroupState(
          cisco::common::deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    case cisco::common::TagValue::MAX_BEYOND_TASK_LIMIT_TAG: {
      result.setMaxBeyondTaskLimit(
          cisco::common::deserialize<std::uint32_t>(floating_data.getData()));
    } break;
    default:
      break;
    }
  }

  return result;
}

#endif