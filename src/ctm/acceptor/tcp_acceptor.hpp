#pragma once

#ifndef _CTM_CTM_ACCEPTOR_TCP_ACCEPTOR_HPP_
#define _CTM_CTM_ACCEPTOR_TCP_ACCEPTOR_HPP_

#include "../../util/ini_loader.h"
#include "../handler/tcp_handler.hpp"
#include "./acceptor.hpp"

#include <asio/awaitable.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/error_code.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/post.hpp>
#include <asio/strand.hpp>
#include <asio/this_coro.hpp>
#include <asio/use_awaitable.hpp>
#include <spdlog/spdlog.h>

#include <exception>
#include <thread>
#include <vector>

namespace ctm::acceptor {
/**
 * @brief TCP 서버 acceptor
 *
 */
class TCPAcceptor : public Acceptor {
public:
  /**
   * @brief Construct a new Asio Acceptor object
   *
   */
  TCPAcceptor()
      : io_context(std::thread::hardware_concurrency()),
        strand(asio::make_strand(io_context)),
        endpoint(asio::ip::tcp::v4(), util::IniLoader::getInstance()->get(
                                          "server", "tcp.port", 5110)),
        acceptor(io_context, endpoint) {
    spdlog::debug("TCP Acceptor constructed.");
  }
  /**
   * @brief Destroy the Asio Acceptor object
   *
   */
  virtual ~TCPAcceptor() = default;

  /**
   * @brief TCP 클라이언트 접속 스레드 실행
   *
   */
  virtual void accept() noexcept override {
    std::thread accept_thread{[&]() {
      spdlog::debug("TCP Acceptor started.");
      startAccept();
      io_context.run();
    }};

    accept_thread.detach();
  }

  /**
   * @brief 이벤트 핸들러
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override {}

protected:
  /**
   * @brief TCP 클라이언트 접속 실행
   *
   */
  void startAccept() {
    spdlog::debug("TCP Acceptor start accept called.");

    asio::co_spawn(strand, listener(), asio::detached);
    // asio::co_spawn(strand, cleanListeners(), asio::detached);
  }

  /**
   * @brief TCP 서버 리스닝 실행
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> listener() {
    while (true) {
      co_await asio::post(strand);
      std::unique_ptr<handler::TCPHandler> &client_handler =
          handler_list.emplace_back(std::make_unique<handler::TCPHandler>(
              std::move(co_await acceptor.async_accept(asio::use_awaitable))));

      asio::co_spawn(strand, client_handler->handleConnection(),
                     [&](const std::exception_ptr e) {
                       std::erase(handler_list, client_handler);
                     });
    }
  }

private:
  asio::io_context io_context;
  asio::strand<asio::io_context::executor_type> strand;
  asio::ip::tcp::endpoint endpoint;
  asio::ip::tcp::acceptor acceptor;
  std::vector<std::unique_ptr<handler::TCPHandler>> handler_list{};
};
} // namespace ctm::acceptor

#endif