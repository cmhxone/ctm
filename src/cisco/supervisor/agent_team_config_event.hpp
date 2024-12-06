#pragma once

#include <cstring>
#ifndef _CTM_CISCO_SUPERVISOR_AGENT_TEAM_CONFIG_EVENT_HPP_
#define _CTM_CISCO_SUPERVISOR_AGENT_TEAM_CONFIG_EVENT_HPP_

#include "../common/floating_data.hpp"
#include "../common/mhdr.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace cisco::supervisor {
class AgentTeamConfigEvent {
public:
  /**
   * @brief Construct a new Agent Team Config Event object
   *
   */
  AgentTeamConfigEvent() {}
  /**
   * @brief Destroy the Agent Team Config Event object
   *
   */
  virtual ~AgentTeamConfigEvent() = default;

  /**
   * @brief Set the MHDR object
   *
   * @return const cisco::common::MHDR
   */
  const cisco::common::MHDR getMHDR() const { return mhdr; }
  /**
   * @brief Get the Peripheral I D object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getPeripheralID() const { return peripheral_id; }
  /**
   * @brief Get the Team I D object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getTeamID() const { return team_id; }
  /**
   * @brief Get the Number Of Agent object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getNumberOfAgent() const { return number_of_agents; }
  /**
   * @brief Get the Config Operation object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getConfigOperation() const {
    return config_operation;
  }
  /**
   * @brief Get the Department I D object
   *
   * @return constexpr std::int32_t
   */
  constexpr std::int32_t getDepartmentID() const { return department_id; }
  /**
   * @brief Get the Agent Team Name object
   *
   * @return constexpr std::string
   */
  constexpr std::string getAgentTeamName() const {
    return agent_team_name.value_or("");
  }
  /**
   * @brief
   *
   * @return constexpr std::string
   */
  constexpr std::string getATCAgentID() const {
    return atc_agent_id.value_or("");
  }
  /**
   * @brief Get the Agent Flag object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getAgentFlag() const {
    return agent_flag.value_or(0);
  }
  /**
   * @brief
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getATCAgentState() const {
    return atc_agent_state.value_or(0);
  }
  /**
   * @brief
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getATCAgentStateDuration() const {
    return atc_agent_state_duration.value_or(0);
  }

  /**
   * @brief
   *
   * @param mhdr
   */
  void setMHDR(const cisco::common::MHDR &mhdr) { this->mhdr = mhdr; }
  /**
   * @brief Set the Peripheral I D object
   *
   * @param peripheral_id
   */
  void setPeripheralID(const std::uint32_t peripheral_id) {
    this->peripheral_id = peripheral_id;
  }
  /**
   * @brief Set the Team I D object
   *
   * @param team_id
   */
  void setTeamID(const std::uint32_t team_id) { this->team_id = team_id; }
  /**
   * @brief Set the Number Of Agents object
   *
   * @param number_of_agents
   */
  void setNumberOfAgents(const std::uint16_t number_of_agents) {
    this->number_of_agents = number_of_agents;
  }
  /**
   * @brief Set the Config Operation object
   *
   * @param config_operation
   */
  void setConfigOperation(const std::uint16_t config_operation) {
    this->config_operation = config_operation;
  }
  /**
   * @brief Set the Department I D object
   *
   * @param department_id
   */
  void setDepartmentID(const std::int32_t department_id) {
    this->department_id = department_id;
  }
  /**
   * @brief Set the Agent Team Name object
   *
   * @param agent_team_name
   */
  void setAgentTeamName(const std::string_view agent_team_name) {
    this->agent_team_name = agent_team_name.data();
  }
  /**
   * @brief
   *
   * @param atc_agent_id
   */
  void setATCAgentID(const std::string_view atc_agent_id) {
    this->atc_agent_id = atc_agent_id.data();
  }
  /**
   * @brief Set the Agent Flag object
   *
   * @param agent_flag
   */
  void setAgentFlag(const std::uint16_t agent_flag) {
    this->agent_flag = agent_flag;
  }
  /**
   * @brief
   *
   * @param atc_agent_state
   */
  void setATCAgentState(const std::uint16_t atc_agent_state) {
    this->atc_agent_state = atc_agent_state;
  }
  /**
   * @brief
   *
   * @param atc_agent_state_duration
   */
  void setATCAgentStateDuration(const std::uint16_t atc_agent_state_duration) {
    this->atc_agent_state_duration = atc_agent_state_duration;
  }

protected:
private:
  // 고정 데이터 영역
  cisco::common::MHDR mhdr;
  std::uint32_t peripheral_id;
  std::uint32_t team_id;
  std::uint16_t number_of_agents;
  std::uint16_t config_operation;
  std::int32_t department_id;
  // 가변 데이터 영역
  std::optional<std::string> agent_team_name;
  std::optional<std::string> atc_agent_id;
  std::optional<std::uint16_t> agent_flag;
  std::optional<std::uint16_t> atc_agent_state;
  std::optional<std::uint16_t> atc_agent_state_duration;
};
} // namespace cisco::supervisor

template <>
inline const cisco::supervisor::AgentTeamConfigEvent
cisco::common::deserialize(const std::vector<std::byte> &bytes) {
  cisco::supervisor::AgentTeamConfigEvent agent_team_config_event{};

  size_t packet_index = 0;
  size_t field_length = 8;

  agent_team_config_event.setMHDR(deserialize<cisco::common::MHDR>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 4;
  agent_team_config_event.setPeripheralID(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 4;
  agent_team_config_event.setTeamID(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 2;
  agent_team_config_event.setNumberOfAgents(deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 2;
  agent_team_config_event.setConfigOperation(deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 4;
  agent_team_config_event.setDepartmentID(deserialize<std::int32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  while (packet_index <
         agent_team_config_event.getMHDR().getMessageLength() + 8) {
    cisco::common::FloatingData floating_data =
        deserialize<cisco::common::FloatingData>(std::vector<std::byte>{
            bytes.cbegin() + packet_index, bytes.cend()});
    packet_index += floating_data.getData().size() + 4;

    switch (floating_data.getTag()) {
    case TagValue::AGENT_TEAM_NAME_TAG: {
      std::string agent_team_name;
      agent_team_name.resize(floating_data.getData().size());
      std::memcpy(agent_team_name.data(), floating_data.getData().data(),
                  floating_data.getData().size());
      agent_team_config_event.setAgentTeamName(agent_team_name);
    } break;
    case TagValue::ATC_AGENT_ID_TAG: {
      std::string agent_team_id;
      agent_team_id.resize(floating_data.getData().size());
      std::memcpy(agent_team_id.data(), floating_data.getData().data(),
                  floating_data.getData().size());
      agent_team_config_event.setATCAgentID(agent_team_id);
    } break;
    case TagValue::AGENT_FLAGS_TAG: {
      agent_team_config_event.setAgentFlag(
          deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    case TagValue::ATC_AGENT_STATE_TAG: {
      agent_team_config_event.setATCAgentState(
          deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    case TagValue::ATC_AGENT_STATE_DURATION_TAG: {
      agent_team_config_event.setATCAgentStateDuration(
          deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    default:
      break;
    }
  }

  return agent_team_config_event;
}

#endif