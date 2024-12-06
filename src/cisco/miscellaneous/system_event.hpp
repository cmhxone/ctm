#pragma once

#ifndef _CTM_CISCO_MISCELLANEOUS_SYSTEM_EVENT_HPP_
#define _CTM_CISCO_MISCELLANEOUS_SYSTEM_EVENT_HPP_

#include "../common/floating_data.hpp"
#include "../common/mhdr.hpp"
#include "../common/time.hpp"

#include <cstdint>
#include <cstring>
#include <optional>
#include <string_view>
#include <vector>

namespace cisco::misc {
class SystemEvent {
public:
  /**
   * @brief Construct a new System Event object
   *
   */
  SystemEvent() {}
  /**
   * @brief Destroy the System Event object
   *
   */
  virtual ~SystemEvent() = default;

  /**
   * @brief Set the MHDR object
   *
   * @return const common::MHDR
   */
  const common::MHDR getMHDR() const { return mhdr; }
  /**
   * @brief Set the PG Status object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getPGStatus() const { return pg_status; }
  /**
   * @brief Set the ICM Central Controller Time object
   *
   * @return constexpr common::Time
   */
  constexpr common::Time getICMCentralControllerTime() const {
    return icm_central_controller_time;
  }
  /**
   * @brief Get the System Event ID object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getSystemEventID() const { return system_event_id; }
  /**
   * @brief Get the System Event Arg1 object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getSystemEventArg1() const {
    return system_event_arg_1;
  }
  /**
   * @brief Get the System Event Arg2 object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getSystemEventArg2() const {
    return system_event_arg_2;
  }
  /**
   * @brief Get the System Event Arg3 object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getSystemEventArg3() const {
    return system_event_arg_3;
  }
  /**
   * @brief Get the Event Device Type object
   *
   * @return constexpr std::uint16_t
   */
  constexpr std::uint16_t getEventDeviceType() const {
    return event_device_type;
  }
  /**
   * @brief Get the Text object
   *
   * @return const std::string
   */
  const std::string getText() const { return text.value_or(""); }
  /**
   * @brief Get the Event Device ID object
   *
   * @return const std::string
   */
  const std::string getEventDeviceID() const {
    return event_device_id.value_or("");
  }

  /**
   * @brief Set the MHDR object
   *
   * @param mhdr
   */
  void setMHDR(const common::MHDR &mhdr) { this->mhdr = mhdr; }
  /**
   * @brief Set the PG Status object
   *
   * @param pg_status
   */
  void setPGStatus(const std::uint32_t pg_status) {
    this->pg_status = pg_status;
  }
  /**
   * @brief Set the ICM Central Controller Time object
   *
   * @param icm_central_controller_time
   */
  void
  setICMCentralControllerTime(const common::Time icm_central_controller_time) {
    this->icm_central_controller_time = icm_central_controller_time;
  }
  /**
   * @brief Set the System Event ID object
   *
   * @param system_event_id
   */
  void setSystemEventID(const std::uint32_t system_event_id) {
    this->system_event_id = system_event_id;
  }
  /**
   * @brief Set the System Event Arg1 object
   *
   * @param system_event_arg_1
   */
  void setSystemEventArg1(const std::uint32_t system_event_arg_1) {
    this->system_event_arg_1 = system_event_arg_1;
  }
  /**
   * @brief Set the System Event Arg2 object
   *
   * @param system_event_arg_2
   */
  void setSystemEventArg2(const std::uint32_t system_event_arg_2) {
    this->system_event_arg_1 = system_event_arg_2;
  }
  /**
   * @brief Set the System Event Arg3 object
   *
   * @param system_event_arg_3
   */
  void setSystemEventArg3(const std::uint32_t system_event_arg_3) {
    this->system_event_arg_3 = system_event_arg_3;
  }
  /**
   * @brief Set the Event Device Type object
   *
   * @param event_device_type
   */
  void setEventDeviceType(const std::uint16_t event_device_type) {
    this->event_device_type = event_device_type;
  }
  /**
   * @brief Set the Text object
   *
   * @param text
   */
  void setText(const std::string_view &text) { this->text = text.data(); }
  /**
   * @brief Set the Event Device ID object
   *
   * @param event_device_id
   */
  void setEventDeviceID(const std::string_view &event_device_id) {
    this->event_device_id = event_device_id.data();
  }

protected:
private:
  // 고정 데이터 영역
  common::MHDR mhdr;
  std::uint32_t pg_status;
  common::Time icm_central_controller_time;
  std::uint32_t system_event_id;
  std::uint32_t system_event_arg_1;
  std::uint32_t system_event_arg_2;
  std::uint32_t system_event_arg_3;
  std::uint16_t event_device_type;
  // 가변 데이터 영역
  std::optional<std::string> text;
  std::optional<std::string> event_device_id;
};
} // namespace cisco::misc

template <>
inline const cisco::misc::SystemEvent
cisco::common::deserialize(const std::vector<std::byte> &bytes) {
  cisco::misc::SystemEvent system_event{};

  size_t packet_index = 0;
  size_t field_size = 8;

  system_event.setMHDR(deserialize<cisco::common::MHDR>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  field_size = 4;
  system_event.setPGStatus(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  system_event.setICMCentralControllerTime(deserialize<cisco::common::Time>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  system_event.setSystemEventID(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  system_event.setSystemEventArg1(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  system_event.setSystemEventArg2(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  system_event.setSystemEventArg3(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  field_size = 2;
  system_event.setEventDeviceType(deserialize<std::uint16_t>(
      std::vector<std::byte>{bytes.cbegin() + packet_index,
                             bytes.cbegin() + packet_index + field_size}));
  packet_index += field_size;

  while (packet_index < system_event.getMHDR().getMessageLength() + 8) {
    cisco::common::FloatingData floating_data =
        deserialize<cisco::common::FloatingData>(std::vector<std::byte>{
            bytes.cbegin() + packet_index, bytes.cend()});

    packet_index += floating_data.getData().size() + 4;

    switch (floating_data.getTag()) {
    case TagValue::TEXT_TAG: {
      std::string text;
      text.resize(floating_data.getData().size());
      std::memcpy(text.data(), floating_data.getData().data(),
                  floating_data.getData().size());
      system_event.setText(text);
    } break;
    case TagValue::EVENT_DEVICE_ID_TAG: {
      std::string event_device_id;
      event_device_id.resize(floating_data.getData().size());
      std::memcpy(event_device_id.data(), floating_data.getData().data(),
                  floating_data.getData().size());
      system_event.setEventDeviceID(event_device_id);
    } break;
    default:
      break;
    }
  }

  return system_event;
}

#endif