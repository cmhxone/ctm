#pragma once

#ifndef _CTM_CISCO_CONTROL_QUERY_AGENT_STATE_REQ_HPP_
#define _CTM_CISCO_CONTROL_QUERY_AGENT_STATE_REQ_HPP_

#include "../common/floating_data.hpp"
#include "../common/mhdr.hpp"
#include "../common/serializable.hpp"

#include <cstddef>
#include <iterator>
#include <optional>
#include <string_view>

namespace cisco::control {
/**
 * @brief QUERY_AGENT_STATE_REQ 전송
 *
 */
class QueryAgentStateReq {
public:
  QueryAgentStateReq() {
    mhdr.setMessageType(common::MessageType::QUERY_AGENT_STATE_REQ);
  }
  virtual ~QueryAgentStateReq() = default;

  /**
   * @brief Get the MHDR object
   *
   * @return const common::MHDR
   */
  const common::MHDR getMHDR() const { return mhdr; };
  /**
   * @brief Get the Invoke ID object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getInvokeID() const { return invoke_id; }
  /**
   * @brief Get the Peripheral ID object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getPeripheralID() const { return peripheral_id; }
  /**
   * @brief Get the MRD ID object
   *
   * @return constexpr std::int32_t
   */
  constexpr std::int32_t getMRDID() const { return mrd_id; }
  /**
   * @brief Get the ICM Agent object
   *
   * @return constexpr std::int32_t
   */
  constexpr std::int32_t getICMAgentID() const { return icm_agent_id; }
  /**
   * @brief Get the Agent Extension object
   *
   * @return const std::optional<std::string>
   */
  const std::optional<std::string> getAgentExtension() const {
    return agent_extension;
  }
  /**
   * @brief Get the Agent ID object
   *
   * @return const std::optional<std::string>
   */
  const std::optional<std::string> getAgentID() const { return agent_id; }
  /**
   * @brief Get the Agent Instrument object
   *
   * @return const std::optional<std::string>
   */
  const std::optional<std::string> getAgentInstrument() const {
    return agent_instrument;
  }

  /**
   * @brief Set the Invoke ID object
   *
   * @param invoke_id
   */
  void setInvokeID(const std::uint32_t invoke_id) {
    this->invoke_id = invoke_id;
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
   * @brief Set the MRD ID object
   *
   * @param mrd_id
   */
  void setMRDID(const std::int32_t mrd_id) { this->mrd_id = mrd_id; }
  /**
   * @brief Set the ICM Agent ID Object
   *
   * @param icm_agent_id
   */
  void setICMAgentID(const std::int32_t icm_agent_id) {
    this->icm_agent_id = icm_agent_id;
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
   * @brief Set the Agent ID object
   *
   * @param agent_id
   */
  void setAgentID(const std::string_view &agent_id) {
    this->agent_id = agent_id.data();
  }
  /**
   * @brief Set the Agent Instrument object
   *
   * @param agent_instrument
   */
  void setAgentInstrument(const std::string_view &agent_instrument) {
    this->agent_instrument = agent_instrument;
  }

protected:
private:
  // 고정 데이터 영역
  common::MHDR mhdr;
  std::uint32_t invoke_id;
  std::uint32_t peripheral_id;
  std::int32_t mrd_id;
  std::int32_t icm_agent_id;
  // 가변 데이터 영역
  std::optional<std::string> agent_extension;
  std::optional<std::string> agent_id;
  std::optional<std::string> agent_instrument;
};
} // namespace cisco::control

template <>
inline const std::vector<std::byte> cisco::common::serialize(
    const cisco::control::QueryAgentStateReq &query_agent_state_req) {
  std::vector<std::byte> body{};

  std::vector<std::vector<std::byte>> members{};

  members.emplace_back(serialize(query_agent_state_req.getInvokeID()));
  members.emplace_back(serialize(query_agent_state_req.getPeripheralID()));
  members.emplace_back(serialize(query_agent_state_req.getMRDID()));
  members.emplace_back(serialize(query_agent_state_req.getICMAgentID()));

  if (query_agent_state_req.getAgentExtension().has_value()) {
    cisco::common::FloatingData agent_extension{};
    agent_extension.setTag(TagValue::AGENT_EXTENSION_TAG);
    agent_extension.setData(
        query_agent_state_req.getAgentExtension().value_or(""));
    members.emplace_back(serialize(agent_extension));
  }

  if (query_agent_state_req.getAgentID().has_value()) {

    cisco::common::FloatingData agent_id{};
    agent_id.setTag(TagValue::AGENT_ID_TAG);
    agent_id.setData(query_agent_state_req.getAgentID().value_or(""));
    members.emplace_back(serialize(agent_id));
  }

  if (query_agent_state_req.getAgentInstrument().has_value()) {
    cisco::common::FloatingData agent_instrument{};
    agent_instrument.setTag(TagValue::AGENT_INSTRUMENT_TAG);
    agent_instrument.setData(
        query_agent_state_req.getAgentInstrument().value_or(""));
    members.emplace_back(serialize(agent_instrument));
  }

  for (const std::vector<std::byte> &member : members) {
    std::move(member.cbegin(), member.cend(), std::back_inserter(body));
  }

  MHDR mhdr = query_agent_state_req.getMHDR();
  mhdr.setMessageLength(body.size());

  std::vector<std::byte> result = serialize(mhdr);
  std::move(body.cbegin(), body.cend(), std::back_inserter(result));

  return result;
}

#endif