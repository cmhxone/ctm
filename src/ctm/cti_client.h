#pragma once

#ifndef _CTM_CTM_CTI_CLIENT_H_
#define _CTM_CTM_CTI_CLIENT_H_

#include "../channel/subscriber.hpp"

#include <Poco/Net/StreamSocket.h>
#include <Poco/Timespan.h>

#include <atomic>
#include <string>


namespace ctm {
class CTIClient : public channel::Subscriber {
public:
  /**
   * @brief Construct a new CTIClient object
   *
   */
  CTIClient();
  /**
   * @brief Destroy the CTIClient object
   *
   */
  virtual ~CTIClient() = default;

  const CTIClient &operator=(const CTIClient &) = delete;
  CTIClient(const CTIClient &) = delete;

  /**
   * @brief CTI 서버 호스트 반환
   *
   * @return const std::string
   */
  const std::string getCTIServerHost() const { return cti_server_host; }

  /**
   * @brief CTI 서버 접속
   *
   */
  void connect() noexcept;
  /**
   * @brief CTI 서버 접속 해제
   *
   */
  void disconnect() noexcept;

  /**
   * @brief 채널 이벤트 핸들링
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override;

protected:
private:
  Poco::Net::StreamSocket client_socket{};
  Poco::Timespan connection_timespan{5'000'000};
  Poco::Timespan heartbeat_timespan{5'000'000};
  std::string cti_server_host;
  std::atomic_bool is_connected{false};
};
} // namespace ctm

#endif