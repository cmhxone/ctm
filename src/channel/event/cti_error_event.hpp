#pragma once

#include <string_view>
#ifndef _CTM_CHANNEL_EVENT_CTI_ERROR_EVENT_HPP_
#define _CTM_CHANNEL_EVENT_CTI_ERROR_EVENT_HPP_

#include "./error_event.hpp"

#include <string>

namespace channel::event {
class CTIErrorEvent : public ErrorEvent {
public:
  /**
   * @brief CTI 오류 유형
   *
   */
  enum class CTIErrorType {
    CONNECTION_FAIL, // 접속 불가
    CONNECTION_LOST, // 비정상 연결 중단
  };

public:
  /**
   * @brief Construct a new CTIErrorEvent object
   *
   */
  CTIErrorEvent(const std::string_view &error_host,
                const CTIErrorType &cti_error_type)
      : ErrorEvent(ErrorType::CTI_ERROR), error_host(error_host.data()),
        cti_error_type(cti_error_type) {};

  /**
   * @brief Destroy the CTIErrorEvent object
   *
   */
  virtual ~CTIErrorEvent() = default;

  /**
   * @brief Get the Event Type object
   *
   * @return constexpr EventType
   */
  virtual constexpr EventType getEventType() const override {
    return ErrorEvent::getEventType();
  }

  /**
   * @brief Get the Error Type object
   *
   * @return constexpr ErrorType
   */
  virtual constexpr ErrorType getErrorType() const override {
    return ErrorType::CTI_ERROR;
  }

  /**
   * @brief Get the Error Host object
   *
   * @return const std::string
   */
  const std::string getErrorHost() const { return error_host; }

  /**
   * @brief CTI 오류 유형을 반환
   *
   * @return constexpr CTIErrorType
   */
  constexpr CTIErrorType getCTIErrorType() const { return cti_error_type; }

protected:
private:
  std::string error_host;
  CTIErrorType cti_error_type;
};
} // namespace channel::event

#endif