#pragma once

#ifndef _CTM_CISCO_COMMON_FLOATING_DATA_HPP_
#define _CTM_CISCO_COMMON_FLOATING_DATA_HPP_

#include "./serializable.hpp"
#include "./tag_value.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <vector>

namespace cisco::common {

/**
 * @brief GED-188 프로토콜 가변 데이터 클래스
 *
 */
class FloatingData {
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
     * @return TagValue
     */
    const TagValue getTag() const { return tag; }

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
    void setTag(const TagValue tag) { this->tag = tag; }

    /**
     * @brief Set the Data object
     *
     * @param data
     */
    void setData(const std::vector<std::byte> &data) { this->data = data; }

    /**
     * @brief Set the Data object
     *
     * @param data
     */
    void setData(const std::string_view &data) {
        this->data.resize(data.length());

        std::memcpy(this->data.data(), data.data(), data.length());
    }

  protected:
  private:
    TagValue tag;
    std::vector<std::byte> data;
};

template <>
inline const std::vector<std::byte>
serialize(const FloatingData &floating_data) {
    std::vector<std::byte> result{};

    const auto tag = serialize(floating_data.getTag());
    const auto length =
        serialize(static_cast<uint16_t>(floating_data.getData().size()));
    const auto data = serialize(floating_data.getData());

    std::move(tag.cbegin(), tag.cend(), std::back_inserter(result));
    std::move(length.cbegin(), length.cend(), std::back_inserter(result));
    std::move(data.cbegin(), data.cend(), std::back_inserter(result));

    return result;
}

template <>
inline const FloatingData deserialize(const std::vector<std::byte> &bytes) {
    FloatingData floating_data{};
    floating_data.setTag(deserialize<TagValue>(
        std::vector<std::byte>(bytes.cbegin(), bytes.cbegin() + 2)));
    uint16_t length = deserialize<std::uint16_t>(
        std::vector<std::byte>(bytes.cbegin() + 2, bytes.cbegin() + 4));
    floating_data.setData(deserialize<std::vector<std::byte>>(
        std::vector<std::byte>(bytes.cbegin() + 4, bytes.cbegin() + length)));

    return floating_data;
}

} // namespace cisco::common

#endif