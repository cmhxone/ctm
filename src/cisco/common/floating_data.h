#pragma once

#ifndef _CTM_CISCO_COMMON_FLOATING_DATA_H_
#define _CTM_CISCO_COMMON_FLOATING_DATA_H_

#include <cstddef>
#include <cstdint>

namespace cisco::common {

/**
 * @brief Floating Data
 *
 */
typedef struct FloatingData {
  std::uint16_t tag;
  std::uint16_t fixed_length;
  std::byte *data;
} FloatingData;

} // namespace cisco::common

#endif