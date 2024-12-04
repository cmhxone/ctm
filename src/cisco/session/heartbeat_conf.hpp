#pragma once

#ifndef _CTM_CISCO_SESSION_HEARTBEAT_CONF_HPP_
#define _CTM_CISCO_SESSION_HEARTBEAT_CONF_HPP_

/*
  HEARTBEAT_CONF 패킷 레이아웃
  +------+------------+
  | MHDR | Fixed Part |
  +------+------------+
*/

#include "../common/mhdr.hpp"
#include "../common/serializable.hpp"

#include <cstdint>

namespace cisco::session {
class HeartbeatConf {
public:
  /**
   * @brief Construct a new Heartbeat Conf object
   *
   */
  HeartbeatConf() {}

  /**
   * @brief Destroy the Heartbeat Conf object
   *
   */
  virtual ~HeartbeatConf() = default;

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
  void setInvokeID(const std::uint32_t &invoke_id) {
    this->invoke_id = invoke_id;
  }

protected:
private:
  common::MHDR mhdr;
  std::uint32_t invoke_id;
};
} // namespace cisco::session

template <>
inline const cisco::session::HeartbeatConf
cisco::common::deserialize(const std::vector<std::byte> &bytes) {

  cisco::session::HeartbeatConf result{};

  result.setMHDR(deserialize<cisco::common::MHDR>(
      std::vector<std::byte>{bytes.cbegin(), bytes.cbegin() + 8}));
  result.setInvokeID(deserialize<std::uint32_t>(
      std::vector<std::byte>{bytes.cbegin() + 8, bytes.cend()}));

  return result;
}

#endif