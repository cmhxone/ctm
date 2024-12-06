#pragma once

#ifndef _CTM_CISCO_COMMON_TIME_HPP_
#define _CTM_CISCO_COMMON_TIME_HPP_

#include <cstdint>

namespace cisco::common {
typedef std::uint32_t Time; // Unix time (seconds)
};
#endif