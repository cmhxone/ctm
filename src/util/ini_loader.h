#pragma once

#ifndef _CTM_UTIL_INI_LOADER_H_
#define _CTM_UTIL_INI_LOADER_H_

#include "../template/singleton.hpp"

#include <INIReader.h>

#include <string_view>

namespace util {
class IniLoader : public tmpl::Singleton<IniLoader> {
  public:
    IniLoader();
    virtual ~IniLoader() = default;

    template <typename T>
    const T get(const std::string_view &section, const std::string_view &key,
                const T default_value) const;

  protected:
  private:
    INIReader reader;
};
} // namespace util

#endif