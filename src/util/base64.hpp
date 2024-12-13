#pragma once

#ifndef _CTM_UTIL_BASE64_HPP_
#define _CTM_UTIL_BASE64_HPP_

#include <sstream>
#include <string>
#include <vector>

namespace util {

static std::string BASE64_CHAR = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789+/";

class Base64 {
public:
  /**
   * @brief Base64 인코딩
   *
   * @param data
   * @return const std::string
   */
  const static std::string
  encode(const std::vector<unsigned char> data) noexcept {
    std::stringstream result;

    std::size_t i = 0;
    unsigned char array_3[3];
    char array_4[4];

    for (unsigned char byte : data) {
      // i => 0인 경우 원시 배열 초기화
      if (i == 0) {
        std::memset(array_3, 0, sizeof(array_3));
      }

      // 원시 배열에 바이트 할당
      array_3[i] = byte;

      // i => 0 ~ 2 순환
      if (i == 2) {
        array_4[0] = BASE64_CHAR[(array_3[0] & 0b1111'1100) >> 2];
        array_4[1] = BASE64_CHAR[((array_3[0] & 0b0000'0011) << 4) |
                                 ((array_3[1] & 0b1111'0000) >> 4)];
        array_4[2] = BASE64_CHAR[((array_3[1] & 0b0000'1111) << 2) |
                                 ((array_3[2] & 0b1100'0000) >> 6)];
        array_4[3] = BASE64_CHAR[(array_3[2] & 0b0011'1111)];

        // 스트림에 인코드 문자열 추가
        result.write(array_4, 4);
        std::memset(array_4, 0, sizeof(array_4));

        i = 0;
      } else {
        ++i;
      }
    }

    // 최종 i => 0가 아닌 경우 패딩 처리
    if (i != 0) {
      array_4[0] = BASE64_CHAR[(array_3[0] & 0b1111'1100) >> 2];
      array_4[1] = BASE64_CHAR[((array_3[0] & 0b0000'0011) << 4) |
                               ((array_3[1] & 0b1111'0000) >> 4)];
      array_4[2] = BASE64_CHAR[((array_3[1] & 0b0000'1111) << 2) |
                               ((array_3[2] & 0b1100'0000) >> 6)];
      array_4[3] = BASE64_CHAR[(array_3[2] & 0b0011'1111)];

      // 스트림에 인코드 문자열 추가
      result.write(array_4, i + 1);
      result.write("==", 3 - i);
    }

    return result.str();
  }

  /**
   * @brief Base64 디코딩
   *
   * @param encode_text
   * @return const std::vector<unsigned char>
   */
  const static std::vector<unsigned char>
  decode(const std::string_view encode_text) noexcept {
    std::vector<unsigned char> result;

    std::size_t i = 0;
    unsigned char array_3[3];
    std::size_t array_4[4];

    for (char ch : encode_text) {
      // i => 0인 경우 원시 배열 초기화
      if (i == 0) {
        std::memset(array_4, 0, sizeof(array_4));
      }

      // 원시 배열에 Base64 문자 인덱스 할당
      array_4[i] = BASE64_CHAR.find(ch);
      // Base64 문자열 외의 데이터가 오는 경우, 루프 종료
      if (array_4[i] == std::string::npos) {
        break;
      }

      // i => 0 ~ 3 순환
      if (i == 3) {
        array_3[0] =
            (static_cast<unsigned char>(array_4[0]) << 2) |
            (static_cast<unsigned char>((array_4[1] & 0b0011'0000) >> 4));
        array_3[1] =
            (static_cast<unsigned char>((array_4[1] & 0b0000'1111) << 4) |
             (static_cast<unsigned char>((array_4[2] & 0b0011'1100) >> 2)));
        array_3[2] =
            (static_cast<unsigned char>((array_4[2] & 0b0000'0011) << 6) |
             (static_cast<unsigned char>(array_4[3])));

        for (int j = 0; j < 3; j++) {
          result.emplace_back(array_3[j]);
        }

        std::memset(array_3, 0, sizeof(array_3));
        i = 0;
      } else {
        ++i;
      }
    }

    // 최종 i => 0이 아닌 경우 Byte처리
    if (i != 0) {
      array_3[0] =
          (static_cast<unsigned char>(array_4[0]) << 2) |
          (static_cast<unsigned char>((array_4[1] & 0b0011'0000) >> 4));
      array_3[1] =
          (static_cast<unsigned char>((array_4[1] & 0b0000'1111) << 4) |
           (static_cast<unsigned char>((array_4[2] & 0b0011'1100) >> 2)));
      array_3[2] =
          (static_cast<unsigned char>((array_4[2] & 0b0000'0011) << 6) |
           (static_cast<unsigned char>(array_4[3])));

      for (int j = 0; j < i - 1; j++) {
        result.emplace_back(array_3[j]);
      }
    }

    return result;
  }

protected:
private:
};
} // namespace util
#endif