#pragma once

#ifndef _CTM_CTM_CTM_H_
#define _CTM_CTM_CTM_H_

#include "../channel/subscriber.hpp"
#include "./acceptor/acceptor.hpp"
#include "./cti_client.h"

#include <memory>
#include <vector>

namespace ctm {
/**
 * @brief CTM 서버 클래스
 *
 */
class CTM : public channel::Subscriber {
public:
  /**
   * @brief Construct a new CTM object
   *
   */
  CTM();

  /**
   * @brief Destroy the CTM object
   *
   */
  virtual ~CTM();

  const CTM &operator=(const CTM &) = delete;
  CTM(const CTM &) = delete;

  /**
   * @brief 이벤트 핸들러
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override;

protected:
private:
  std::unique_ptr<CTIClient> cti_client;
  std::vector<std::unique_ptr<acceptor::Acceptor>> acceptors;
};
} // namespace ctm

#endif