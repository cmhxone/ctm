#pragma once

#ifndef _CTM_CISCO_SESSION_OPEN_CONF_HPP_
#define _CTM_CISCO_SESSION_OPEN_CONF_HPP_

#include "../common/floating_data.hpp"
#include "../common/mhdr.hpp"
#include "../common/time.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string_view>
#include <vector>

namespace cisco::session {
class OpenConf {
public:
  OpenConf() {}
  virtual ~OpenConf() {}

  const cisco::common::MHDR getMHDR() const { return mhdr; }
  constexpr std::uint32_t getInvokeID() const { return invoke_id; }
  constexpr std::uint32_t getServiceGranted() const { return service_granted; }
  constexpr std::uint32_t getMonitorID() const { return monitor_id; }
  constexpr std::uint32_t getPGStatus() const { return pg_status; }
  constexpr cisco::common::Time getICMCentralControllerTime() const {
    return icm_central_controller_time;
  }
  constexpr bool getPeripheralOnline() const { return peripheral_online; }
  constexpr std::uint16_t getPeripheralType() const { return peripheral_type; }
  constexpr std::uint16_t getAgentState() const { return agent_state; }
  constexpr std::int32_t getDepartmentID() const { return department_id; }
  constexpr std::uint16_t getSessionType() const { return session_type; }
  const std::string getAgentExtension() const {
    return agent_extension.value_or("");
  }
  const std::string getAgentID() const { return agent_id.value_or(""); }
  const std::string getAgentInstrument() const {
    return agent_instrument.value_or("");
  }
  constexpr std::uint16_t getNumPeripherals() const {
    return num_peripherals.value_or(0);
  }
  constexpr std::uint32_t getFltPeripheralID() const {
    return flt_peripheral_id.value_or(0);
  }
  constexpr std::uint16_t getMultilineAgentControl() const {
    return multiline_agent_control.value_or(0);
  }

  void setMHDR(const cisco::common::MHDR &mhdr) { this->mhdr = mhdr; }
  void setInvokeID(const std::uint32_t invoke_id) {
    this->invoke_id = invoke_id;
  }
  void setServiceGranted(const std::uint32_t service_granted) {
    this->service_granted = service_granted;
  }
  void setMonitorID(const std::uint32_t monitor_id) {
    this->monitor_id = monitor_id;
  }
  void setPGStatus(const std::uint32_t pg_status) {
    this->pg_status = pg_status;
  }
  void setICMCentralControllerTime(
      const cisco::common::Time icm_central_controller_time) {
    this->icm_central_controller_time = icm_central_controller_time;
  }
  void setPeripheralOnline(const bool peripheral_online) {
    this->peripheral_online = peripheral_online;
  }
  void setPeripheralType(const std::uint16_t peripheral_type) {
    this->peripheral_type = peripheral_type;
  }
  void setAgentState(const std::uint16_t agent_state) {
    this->agent_state = agent_state;
  }
  void setDepartmentID(const std::int32_t department_id) {
    this->department_id = department_id;
  }
  void setSessionType(const std::uint16_t session_type) {
    this->session_type = session_type;
  }
  void setAgentExtension(const std::string_view &agent_extension) {
    this->agent_extension = agent_extension.data();
  }
  void setAgentID(const std::string_view &agent_id) {
    this->agent_id = agent_id.data();
  }
  void setAgentInstrument(const std::string_view &agent_instrument) {
    this->agent_instrument = agent_instrument.data();
  }
  void setNumPeripherals(const std::uint16_t num_peripherals) {
    this->num_peripherals = num_peripherals;
  }
  void setFltPeripheralID(const std::uint32_t flt_peripheral_id) {
    this->flt_peripheral_id = flt_peripheral_id;
  }
  void setMultilineAgentControl(const std::uint16_t multiline_agent_control) {
    this->multiline_agent_control = multiline_agent_control;
  }

protected:
private:
  // 고정 데이터 영역
  cisco::common::MHDR mhdr;
  std::uint32_t invoke_id;
  std::uint32_t service_granted;
  std::uint32_t monitor_id;
  std::uint32_t pg_status;
  cisco::common::Time icm_central_controller_time;
  bool peripheral_online;
  std::uint16_t peripheral_type;
  std::uint16_t agent_state;
  std::int32_t department_id;
  std::uint16_t session_type;
  // 가변 데이터 영역
  std::optional<std::string> agent_extension;
  std::optional<std::string> agent_id;
  std::optional<std::string> agent_instrument;
  std::optional<std::uint16_t> num_peripherals;
  std::optional<std::uint32_t> flt_peripheral_id;
  std::optional<std::uint16_t> multiline_agent_control;
};
} // namespace cisco::session

template <>
inline const cisco::session::OpenConf
cisco::common::deserialize(const std::vector<std::byte> &bytes) {
  cisco::session::OpenConf open_conf{};

  size_t packet_index = 0;
  size_t field_length = 8;

  open_conf.setMHDR(deserialize<cisco::common::MHDR>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 4;
  open_conf.setInvokeID(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 4;
  open_conf.setServiceGranted(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 4;
  open_conf.setMonitorID(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 4;
  open_conf.setPGStatus(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 4;
  open_conf.setICMCentralControllerTime(deserialize<cisco::common::Time>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 2;
  open_conf.setPeripheralOnline(deserialize<bool>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 2;
  open_conf.setPeripheralType(deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 2;
  open_conf.setAgentState(deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 4;
  open_conf.setDepartmentID(deserialize<std::int32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  field_length = 2;
  open_conf.setSessionType(deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_length}));
  packet_index += field_length;

  while (packet_index < open_conf.getMHDR().getMessageLength() + 8) {
    const cisco::common::FloatingData floating_data =
        deserialize<cisco::common::FloatingData>(std::vector<std::byte>{
            bytes.cbegin() + packet_index, bytes.cend()});
    packet_index += 4 + floating_data.getData().size();

    switch (floating_data.getTag()) {
    case TagValue::AGENT_EXTENSION_TAG: {
      std::string agent_extension{};
      agent_extension.resize(floating_data.getData().size());
      std::memcpy(agent_extension.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      open_conf.setAgentExtension(agent_extension);
    } break;
    case TagValue::AGENT_ID_TAG: {
      std::string agent_id{};
      agent_id.resize(floating_data.getData().size());
      std::memcpy(agent_id.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      open_conf.setAgentID(agent_id);
    } break;
    case TagValue::AGENT_INSTRUMENT_TAG: {
      std::string agent_instrument{};
      agent_instrument.resize(floating_data.getData().size());
      std::memcpy(agent_instrument.data(), floating_data.getData().data(),
                  floating_data.getData().size());

      open_conf.setAgentInstrument(agent_instrument);
    } break;
    case TagValue::NUM_PERIPHERALS_TAG: {
      open_conf.setNumPeripherals(
          deserialize<std::uint16_t>(floating_data.getData()));
    } break;
    case TagValue::MULTI_LINE_AGENT_CONTROL_TAG: {
      open_conf.setMultilineAgentControl(
          deserialize<std::uint16_t>(floating_data.getData()));
    } break;
      // flt_peripheral_id 태그는 문서상 정의되지 않음...
    default:
      break;
    }
  }

  return open_conf;
}

#endif