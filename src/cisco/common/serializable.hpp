#pragma once

#ifndef _CTM_CISCO_COMMON_SERIALIZABLE_HPP_
#define _CTM_CISCO_COMMON_SERIALIZABLE_HPP_

#include <cstddef>
#include <cstdint>
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
    virtual const std::vector<std::byte> serialize(const T &t) const = 0;

    /**
     * @brief 역직렬화 (패킷 -> 클래스)
     *
     * @param bytes
     * @return const T
     */
    virtual void deserialize(const std::vector<std::byte> &bytes) = 0;

  protected:
  private:
};

template <>
const std::vector<std::byte>
Serializable<std::uint32_t>::serialize(const uint32_t &value) const {
    std::byte bytes[4];

    bytes[0] = static_cast<std::byte>((value & 0xF000) >> 24);
    bytes[1] = static_cast<std::byte>((value & 0x0F00) >> 16);
    bytes[2] = static_cast<std::byte>((value & 0x00F0) >> 8);
    bytes[3] = static_cast<std::byte>((value & 0x000F));

    std::vector<std::byte> result{};

    for (int i = 0; i < 4; i++) {
        result.emplace_back(bytes[i]);
    }

    return result;
}

} // namespace cisco::common

#endif