#pragma once

#ifndef _CTM_CISCO_COMMON_SERIALIZABLE_HPP_
#define _CTM_CISCO_COMMON_SERIALIZABLE_HPP_

#include <cstddef>
#include <vector>

namespace cisco::common {

/**
 * @brief 네트워크 패킷 형태로 직렬화/역직렬화 가능한 클래스 템플릿
 *
 * @tparam T
 */
template <typename T> class Serializable {
public:
  /**
   * @brief Construct a new Serializable object
   *
   */
  Serializable() {};

  /**
   * @brief Destroy the Serializable object
   *
   */
  virtual ~Serializable() {};

  /**
   * @brief 직렬화 (클래스 -> 패킷)
   *
   * @return const std::vector<std::byte>
   */
  virtual const std::vector<std::byte> serialize() const = 0;

  /**
   * @brief 역직렬화 (패킷 -> 클래스)
   *
   * @param bytes
   * @return const T
   */
  virtual const T deserialize(const std::vector<std::byte> &bytes) const = 0;

protected:
private:
};

} // namespace cisco::common

#endif