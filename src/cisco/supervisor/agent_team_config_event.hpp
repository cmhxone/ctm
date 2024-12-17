#pragma once

#ifndef _CTM_CISCO_SUPERVISOR_AGENT_TEAM_CONFIG_EVENT_HPP_
#define _CTM_CISCO_SUPERVISOR_AGENT_TEAM_CONFIG_EVENT_HPP_

#include "../common/floating_data.hpp"
#include "../common/mhdr.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string_view>
#include <vector>

namespace cisco::supervisor {
struct ATCAgent {
  std::string atc_agent_id;
  std::uint16_t agent_flag;
  std::uint16_t atc_agent_state;
  std::uint16_t atc_agent_state_duration;
};

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
   * @brief Get the ATC Agent List object
   *
   * @return const std::vector<ATCAgent>
   */
  const std::vector<ATCAgent> getATCAgentList() const { return atc_agent_list; }

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
   * @brief Set the ATC Agent List object
   *
   * @param atc_agent_list
   */
  void setATCAgentList(const std::vector<ATCAgent> &atc_agent_list) {
    this->atc_agent_list = atc_agent_list;
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
  std::vector<ATCAgent> atc_agent_list{};
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

  std::vector<supervisor::ATCAgent> atc_agent_list{};
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
      std::string agent_id;
      agent_id.resize(floating_data.getData().size());
      std::memcpy(agent_id.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      supervisor::ATCAgent atc_agent{};
      atc_agent.atc_agent_id = agent_id;

      atc_agent_list.emplace_back(atc_agent);
    } break;
    case TagValue::AGENT_FLAGS_TAG: {
      atc_agent_list.at(atc_agent_list.size() - 1).agent_flag =
          deserialize<std::uint16_t>(floating_data.getData());
    } break;
    case TagValue::ATC_AGENT_STATE_TAG: {
      atc_agent_list.at(atc_agent_list.size() - 1).atc_agent_state =
          deserialize<std::uint16_t>(floating_data.getData());
    } break;
    case TagValue::ATC_AGENT_STATE_DURATION_TAG: {
      atc_agent_list.at(atc_agent_list.size() - 1).atc_agent_state_duration =
          deserialize<std::uint16_t>(floating_data.getData());
    } break;
    default:
      break;
    }
  }
  agent_team_config_event.setATCAgentList(atc_agent_list);

  return agent_team_config_event;
}

#endif