#pragma once

#ifndef _CTM_CISCO_COMMON_SERIALIZABLE_HPP_
#define _CTM_CISCO_COMMON_SERIALIZABLE_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cisco::common {

/**
 * @brief 데이터 -> 패킷 직렬화 함수 템플릿
 *
 * @tparam T
 * @param t
 * @return const std::vector<std::byte>
 */
template <typename T> inline const std::vector<std::byte> serialize(const T &t);

template <typename T>
inline const std::vector<std::byte> serialize(const std::optional<T> &t) {
  if (!t.has_value()) {
    return std::vector<std::byte>{};
  }

  return serialize<T>(t.value());
}

template <> inline const std::vector<std::byte> serialize(const char &value) {
  std::vector<std::byte> result{};
  result.emplace_back(static_cast<std::byte>(value));

  return result;
}

template <>
inline const std::vector<std::byte> serialize(const std::byte &value) {
  std::vector<std::byte> result{};
  result.emplace_back(value);

  return result;
}

template <> inline const std::vector<std::byte> serialize(const bool &value) {
  std::vector<std::byte> result{};
  result.emplace_back(static_cast<std::byte>(0x00));
  result.emplace_back(static_cast<std::byte>(value));

  return result;
}

template <>
inline const std::vector<std::byte> serialize(const std::int16_t &value) {
  std::vector<std::byte> result{};

  std::array<std::byte, 2> bytes;
  bytes[0] = static_cast<std::byte>((value & 0xFF'00) >> 8);
  bytes[1] = static_cast<std::byte>((value & 0x00'FF));

  for (const std::byte b : bytes) {
    result.emplace_back(b);
  }

  return result;
}

template <>
inline const std::vector<std::byte> serialize(const std::uint16_t &value) {
  std::vector<std::byte> result{};

  std::array<std::byte, 2> bytes;
  bytes[0] = static_cast<std::byte>((value & 0xFF'00) >> 8);
  bytes[1] = static_cast<std::byte>((value & 0x00'FF));

  for (const std::byte b : bytes) {
    result.emplace_back(b);
  }

  return result;
}

template <>
inline const std::vector<std::byte> serialize(const std::int32_t &value) {
  std::vector<std::byte> result{};

  std::array<std::byte, 4> bytes;
  bytes[0] = static_cast<std::byte>((value & 0xFF'00'00'00) >> 24);
  bytes[1] = static_cast<std::byte>((value & 0x00'FF'00'00) >> 16);
  bytes[2] = static_cast<std::byte>((value & 0x00'00'FF'00) >> 8);
  bytes[3] = static_cast<std::byte>((value & 0x00'00'00'FF));

  for (const std::byte b : bytes) {
    result.emplace_back(b);
  }

  return result;
}

template <>
inline const std::vector<std::byte> serialize(const std::uint32_t &value) {
  std::vector<std::byte> result{};

  std::array<std::byte, 4> bytes;
  bytes[0] = static_cast<std::byte>((value & 0xFF'00'00'00) >> 24);
  bytes[1] = static_cast<std::byte>((value & 0x00'FF'00'00) >> 16);
  bytes[2] = static_cast<std::byte>((value & 0x00'00'FF'00) >> 8);
  bytes[3] = static_cast<std::byte>((value & 0x00'00'00'FF));

  for (const std::byte b : bytes) {
    result.emplace_back(b);
  }

  return result;
}

template <>
inline const std::vector<std::byte> serialize(const std::string_view &value) {
  std::vector<std::byte> result{};
  result.resize(value.length());

  std::memcpy(result.data(), value.data(), value.length());

  return result;
}

template <>
inline const std::vector<std::byte>
serialize(const std::vector<std::byte> &value) {
  return value;
}

/**
 * @brief 패킷 -> 데이터 역직렬화 함수 템플릿
 *
 * @tparam T
 * @param bytes
 * @return const T
 */
template <typename T>
inline const T deserialize(const std::vector<std::byte> &bytes);

template <> inline const bool deserialize(const std::vector<std::byte> &bytes) {
  return static_cast<bool>(bytes.at(0) | bytes.at(1));
}

template <> inline const char deserialize(const std::vector<std::byte> &bytes) {
  return static_cast<char>(bytes.at(0));
}

template <>
inline const std::byte deserialize(const std::vector<std::byte> &bytes) {
  return bytes.at(0);
}

template <>
inline const std::int16_t deserialize(const std::vector<std::byte> &bytes) {
  std::int16_t result = 0;

  result |= (static_cast<std::int32_t>(bytes.at(0)) << 8);
  result |= (static_cast<std::int32_t>(bytes.at(1)));

  return result;
}

template <>
inline const std::uint16_t deserialize(const std::vector<std::byte> &bytes) {
  std::uint16_t result = 0;

  result |= (static_cast<std::uint32_t>(bytes.at(0)) << 8);
  result |= (static_cast<std::uint32_t>(bytes.at(1)));

  return result;
}

template <>
inline const std::int32_t deserialize(const std::vector<std::byte> &bytes) {
  std::int32_t result = 0;

  result |= (static_cast<std::int32_t>(bytes.at(0)) << 24);
  result |= (static_cast<std::int32_t>(bytes.at(1)) << 16);
  result |= (static_cast<std::int32_t>(bytes.at(2)) << 8);
  result |= (static_cast<std::int32_t>(bytes.at(3)));

  return result;
}

template <>
inline const std::uint32_t deserialize(const std::vector<std::byte> &bytes) {
  std::uint32_t result = 0;

  result |= (static_cast<std::uint32_t>(bytes.at(0)) << 24);
  result |= (static_cast<std::uint32_t>(bytes.at(1)) << 16);
  result |= (static_cast<std::uint32_t>(bytes.at(2)) << 8);
  result |= (static_cast<std::uint32_t>(bytes.at(3)));

  return result;
}

template <>
inline const std::string deserialize(const std::vector<std::byte> &bytes) {
  std::string result{};
  result.resize(bytes.size());

  std::memcpy(result.data(), bytes.data(), bytes.size());

  return result;
}

template <>
inline const std::vector<std::byte>
deserialize(const std::vector<std::byte> &bytes) {
  return bytes;
}

} // namespace cisco::common
#endif