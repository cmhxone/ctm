#pragma once

#ifndef _CTM_TEMPLATE_SINGLETON_HPP_
#define _CTM_TEMPLATE_SINGLETON_HPP_

namespace tmpl {
/**
 * @brief 템플릿 싱글턴
 *
 * @tparam T
 */
template <typename T> class Singleton {
public:
  /**
   * @brief Get the Instance object
   *
   * @return T*
   */
  T *getInstance() {
    if (instance == nullptr) {
      instance = new T();
    }

    return instance;
  }

protected:
  /**
   * @brief Construct a new Singleton< T> object
   *
   */
  Singleton<T>() {}

  /**
   * @brief Destroy the Singleton< T> object
   *
   */
  virtual ~Singleton<T>() = default;

private:
  /**
   * @brief 싱글턴 인스턴스
   *
   */
  static T *instance;
};

/**
 * @brief 인스턴스 멤버 nullptr로 초기화
 *
 * @tparam T
 */
template <typename T> T *Singleton<T>::instance = nullptr;
} // namespace tmpl

#endif