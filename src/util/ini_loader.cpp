#include "ini_loader.h"

#include <INIReader.h>

#include <cmath>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

using namespace std;

namespace util {
IniLoader::IniLoader() : reader(filesystem::path{"./res/conf/ctm.ini"}) {}

template <>
const string IniLoader::get(const string_view &section, const string_view &key,
                            const string default_value) const {
    return this->reader.GetString(section.data(), key.data(), default_value);
}

template <>
const bool IniLoader::get(const string_view &section, const string_view &key,
                          const bool default_value) const {
    return this->reader.GetBoolean(section.data(), key.data(), default_value);
}

template <>
const int8_t IniLoader::get(const string_view &section, const string_view &key,
                            const int8_t default_value) const {
    return this->reader.GetInteger(section.data(), key.data(), default_value);
}

template <>
const int16_t IniLoader::get(const string_view &section, const string_view &key,
                             const int16_t default_value) const {
    return this->reader.GetInteger(section.data(), key.data(), default_value);
}

template <>
const int32_t IniLoader::get(const string_view &section, const string_view &key,
                             const int32_t default_value) const {
    return this->reader.GetInteger(section.data(), key.data(), default_value);
}

template <>
const int64_t IniLoader::get(const string_view &section, const string_view &key,
                             const int64_t default_value) const {
    return this->reader.GetInteger64(section.data(), key.data(), default_value);
}

template <>
const uint8_t IniLoader::get(const string_view &section, const string_view &key,
                             const uint8_t default_value) const {
    return this->reader.GetUnsigned(section.data(), key.data(), default_value);
}

template <>
const uint16_t IniLoader::get(const string_view &section,
                              const string_view &key,
                              const uint16_t default_value) const {
    return this->reader.GetUnsigned(section.data(), key.data(), default_value);
}

template <>
const uint32_t IniLoader::get(const string_view &section,
                              const string_view &key,
                              const uint32_t default_value) const {
    return this->reader.GetUnsigned(section.data(), key.data(), default_value);
}

template <>
const uint64_t IniLoader::get(const string_view &section,
                              const string_view &key,
                              const uint64_t default_value) const {
    return this->reader.GetUnsigned64(section.data(), key.data(),
                                      default_value);
}

template <>
const float_t IniLoader::get(const string_view &section, const string_view &key,
                             const float_t default_value) const {
    return this->reader.GetReal(section.data(), key.data(), default_value);
}

template <>
const double_t IniLoader::get(const string_view &section,
                              const string_view &key,
                              const double_t default_value) const {
    return this->reader.GetReal(section.data(), key.data(), default_value);
}

} // namespace util