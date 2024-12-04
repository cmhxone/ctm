#pragma once

#ifndef _CTM_CTM_HANDLER_HANDLER_HPP_
#define _CTM_CTM_HANDLER_HANDLER_HPP_

#include <Poco/AutoPtr.h>
#include <Poco/NObserver.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/StreamSocket.h>

#include <cstddef>
#include <vector>

namespace ctm::handler {
/**
 * @brief 서버 소켓 핸들러, ServerSocketAcceptor에 의해 클라이언트 커넥션이
 * 생성되는 순간 세션별로 하나의 인스턴스가 생성된다
 *
 */
class Handler {
public:
  /**
   * @brief Construct a new Handler object
   *
   * @param socket
   * @param reactor
   */
  Handler(Poco::Net::StreamSocket &socket, Poco::Net::SocketReactor &reactor)
      : socket(socket), reactor(reactor), receive_buffer(4'096) {
    reactor.addEventHandler(
        socket, Poco::NObserver<Handler, Poco::Net::ReadableNotification>{
                    *this, &Handler::onReadable});
    reactor.addEventHandler(
        socket, Poco::NObserver<Handler, Poco::Net::ErrorNotification>{
                    *this, &Handler::onError});
    reactor.addEventHandler(
        socket, Poco::NObserver<Handler, Poco::Net::ShutdownNotification>{
                    *this, &Handler::onShutdown});
  }

  /**
   * @brief Destroy the Handler object
   *
   */
  virtual ~Handler() = default;

  /**
   * @brief 패킷 수신 핸들러
   *
   * @param notification
   */
  virtual void onReadable(
      const Poco::AutoPtr<Poco::Net::ReadableNotification> &notification) = 0;

  /**
   * @brief 오류 핸들러
   *
   * @param notification
   */
  virtual void
  onError(const Poco::AutoPtr<Poco::Net::ErrorNotification> &notification) = 0;

  /**
   * @brief 종료 핸들러
   *
   * @param notification
   */
  virtual void onShutdown(
      const Poco::AutoPtr<Poco::Net::ShutdownNotification> &notification) = 0;

protected:
  Poco::Net::StreamSocket socket;
  Poco::Net::SocketReactor &reactor;
  std::vector<std::byte> receive_buffer;

private:
};
} // namespace ctm::handler

#endif