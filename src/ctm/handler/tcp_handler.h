#pragma once

#ifndef _CTM_CTM_HANDLER_TCP_HANDLER_H_
#define _CTM_CTM_HANDLER_TCP_HANDLER_H_

#include "./handler.hpp"

#include <Poco/AutoPtr.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/StreamSocket.h>

namespace ctm::handler {
class TCPHandler : public Handler {
public:
  /**
   * @brief Construct a new TCPHandler object
   *
   * @param socket
   * @param reactor
   */
  TCPHandler(Poco::Net::StreamSocket &socket,
             Poco::Net::SocketReactor &reactor);

  /**
   * @brief Destroy the TCPHandler object
   *
   */
  virtual ~TCPHandler();

  /**
   * @brief 읽어들일 패킷이 존재하는 경우
   *
   * @param notification
   */
  virtual void onReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>
                              &notification) override;

  /**
   * @brief 오류가 발생한 경우
   *
   * @param notification
   */
  virtual void onError(
      const Poco::AutoPtr<Poco::Net::ErrorNotification> &notification) override;

  /**
   * @brief 커넥션이 종료된 경우
   *
   * @param notification
   */
  virtual void onShutdown(const Poco::AutoPtr<Poco::Net::ShutdownNotification>
                              &notification) override;

  /**
   * @brief 이벤트 핸들러
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override;

protected:
private:
};
} // namespace ctm::handler

#endif