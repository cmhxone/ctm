#pragma once

#include <vector>
#ifndef _CTM_CISCO_COMMON_FLOATING_DATA_H_
#define _CTM_CISCO_COMMON_FLOATING_DATA_H_

#include <cstddef>
#include <cstdint>

#include "./serializable.hpp"

namespace cisco::common {

/**
 * @brief GED-188 프로토콜 가변 데이터 클래스
 *
 */
class FloatingData : public Serializable<FloatingData> {
public:
  /**
   * @brief Construct a new Floating Data object
   *
   */
  FloatingData() {};

  /**
   * @brief Destroy the Floating Data object
   *
   */
  virtual ~FloatingData() = default;

  /**
   * @brief Get the Tag object
   *
   * @return const std::uint16_t
   */
  const std::uint16_t getTag() const { return tag; }

  /**
   * @brief Get the Length object
   *
   * @return const std::uint16_t
   */
  const std::uint16_t getLength() const { return fixed_length; }

  /**
   * @brief Get the Data object
   *
   * @return const std::vector<std::byte>
   */
  const std::vector<std::byte> getData() const { return data; }

  /**
   * @brief Set the Tag object
   *
   * @param tag
   */
  void setTag(const std::uint16_t tag) { this->tag = tag; }

  /**
   * @brief Set the Length object
   *
   * @param length
   */
  void setLength(const std::uint16_t length) { this->fixed_length = length; }

  /**
   * @brief Set the Data object
   *
   * @param data
   */
  void setData(const std::vector<std::byte> &data) { this->data = data; }

  /**
   * @brief 패킷 직렬화 (클래스 -> 패킷)
   *
   * @return const std::vector<std::byte>
   */
  const std::vector<std::byte> serialize() const override;

  /**
   * @brief 패킷 역직렬화 (패킷 -> 클래스)
   *
   * @param bytes
   * @return const FloatingData
   */
  void deserialize(const std::vector<std::byte> &bytes) override;

protected:
private:
  std::uint16_t tag;
  std::uint16_t fixed_length;
  std::vector<std::byte> data;
};

} // namespace cisco::common

#endif