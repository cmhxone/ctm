#pragma once

#ifndef _CTM_CTM_CTI_CLIENT_H_
#define _CTM_CTM_CTI_CLIENT_H_

#include "../channel/subscriber.hpp"

#include <Poco/AutoPtr.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Thread.h>
#include <Poco/Timespan.h>

#include <atomic>
#include <cstddef>
#include <string>
#include <vector>

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
  virtual ~CTIClient();

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

protected:
  void onReadableNotification(
      const Poco::AutoPtr<Poco::Net::ReadableNotification> &notification);
  void onErrorNotification(
      const Poco::AutoPtr<Poco::Net::ErrorNotification> &notification);
  void onShutdownNotification(
      const Poco::AutoPtr<Poco::Net::ShutdownNotification> &notification);

private:
  enum class FiniteState : std::int32_t {
    INITIALIZED, // 초기화 상태
    CONNECTING,  // 접속 시도중 상태
    CONNECTED,   // 접속 성공 상태
    FINISHED,    // 접속 중단 상태
  };

  /**
   * @brief Get the Invoke ID object
   *
   * @return constexpr std::uint32_t
   */
  constexpr std::uint32_t getInvokeID() const {
    return invoke_id.load(std::memory_order_acquire);
  }

  /**
   * @brief Get the Current State object
   *
   * @return constexpr FiniteState
   */
  constexpr FiniteState getCurrentState() const {
    return current_state.load(std::memory_order_acquire);
  }

  /**
   * @brief Get the Peripheral ID object
   *
   * @return constexpr std::int32_t
   */
  constexpr std::int32_t getPeripheralID() const {
    return peripheral_id.load(std::memory_order_acquire);
  }

  /**
   * @brief InvokeID 값 증가
   *
   */
  void addInvokeID() {
    invoke_id.store(getInvokeID() + 1, std::memory_order_release);
  }

  /**
   * @brief 이벤트 핸들링
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override;

  /**
   * @brief Set the Peripheral ID object
   *
   * @param peripheral_id
   */
  void setPeripheralID(const std::int32_t peripheral_id) {
    this->peripheral_id.store(peripheral_id, std::memory_order_release);
  }

private:
  Poco::Net::StreamSocket client_socket{};
  Poco::Net::SocketReactor client_socket_reactor{};
  Poco::Timespan connection_timespan{5'000'000};
  Poco::Timespan heartbeat_timespan{5'000'000};
  std::string cti_server_host;

  std::vector<std::byte> receive_buffer{4'096};
  std::atomic_uint32_t invoke_id{0};
  std::atomic<FiniteState> current_state{FiniteState::INITIALIZED};
  std::atomic_int32_t peripheral_id{5000};

  Poco::Thread reactor_thread;
};
} // namespace ctm

#endif