#pragma once

#ifndef _CTM_CTM_ACCEPTOR_ACCEPTOR_HPP_
#define _CTM_CTM_ACCEPTOR_ACCEPTOR_HPP_

#include "../../channel/subscriber.hpp"

namespace ctm::acceptor {
/**
 * @brief 클라이언트 수신 서버 추상 클래스
 *
 */
class Acceptor : channel::Subscriber {
public:
  /**
   * @brief Construct a new Acceptor object
   *
   */
  Acceptor() {};

  /**
   * @brief Destroy the Acceptor object
   *
   */
  virtual ~Acceptor() = default;

  Acceptor(const Acceptor &) = delete;
  const Acceptor &operator=(const Acceptor &) = delete;

  virtual void accept() noexcept = 0;

protected:
private:
};
} // namespace ctm

#endif