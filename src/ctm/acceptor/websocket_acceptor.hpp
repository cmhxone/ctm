#pragma once

#ifndef _CTM_CTM_ACCEPTOR_WEBSOCKET_ACCEPTOR_HPP_
#define _CTM_CTM_ACCEPTOR_WEBSOCKET_ACCEPTOR_HPP_

#include "../../util/ini_loader.h"
#include "../handler/websocket_handler.hpp"
#include "./acceptor.hpp"

#include <asio/awaitable.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/ssl/context.hpp>
#include <asio/ssl/stream.hpp>
#include <asio/ssl/stream_base.hpp>
#include <asio/this_coro.hpp>
#include <asio/use_awaitable.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <memory>
#include <optional>
#include <thread>
#include <unordered_map>

namespace ctm::acceptor {
class WebsocketAcceptor : public Acceptor {
public:
  /**
   * @brief Construct a new Websocket Acceptor object
   *
   */
  WebsocketAcceptor()
      : io_context(std::thread::hardware_concurrency()),
        endpoint(asio::ip::tcp::v4(), util::IniLoader::getInstance()->get(
                                          "server", "websocket.port", 8085)),
        acceptor(io_context, endpoint) {
    spdlog::debug("Websocket Acceptor constructred.");

    bool ssl_enabled = util::IniLoader::getInstance()->get(
        "server", "websocket.protocol.secure", false);

    // SSL 인증서 적용 여부
    if (ssl_enabled) {
      ssl_context =
          std::make_optional(asio::ssl::context{asio::ssl::context::sslv23});

      std::filesystem::path cert_path{util::IniLoader::getInstance()->get(
          "server", "websocket.protocol.tls.cert.file",
          std::string("./res/ssl/server.crt"))};
      std::filesystem::path key_path{util::IniLoader::getInstance()->get(
          "server", "websocket.protocol.tls.key.file",
          std::string("./res/ssl/server.key"))};

      ssl_context->use_certificate_chain_file(cert_path.string());
      ssl_context->use_private_key_file(key_path.string(),
                                        asio::ssl::context::pem);
    }
  }
  /**
   * @brief Destroy the Websocket Acceptor object
   *
   */
  virtual ~WebsocketAcceptor() = default;

  /**
   * @brief 웹 소켓 클라이언트 접속 스레드 실행
   *
   */
  virtual void accept() noexcept override {
    std::thread accept_thread([&]() {
      spdlog::debug("Websocket Acceptor started.");
      startAccept();
      io_context.run();
    });

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
   * @brief 웹 소켓 클라이언트 접속 실행
   *
   */
  void startAccept() {
    spdlog::debug("Websocket Acceptor start accept called.");

    asio::co_spawn(io_context, listener(), asio::detached);
  }

  /**
   * @brief 웹 소켓 서버 리스닝 실행
   *
   * @return asio::awaitable<void>
   */
  asio::awaitable<void> listener() {
    std::uint64_t id = 0;

    while (true) {
      std::uint64_t id_clone = id++;

      if (ssl_context.has_value()) {
        std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> ssl_socket =
            std::make_shared<asio::ssl::stream<asio::ip::tcp::socket>>(
                acceptor.get_executor(), ssl_context.value());

        spdlog::debug("\tSSL Websocket start accept");
        co_await acceptor.async_accept(ssl_socket->lowest_layer(),
                                       asio::use_awaitable);

        spdlog::debug("\tSSL Websocket start handshake");
        try {
          co_await ssl_socket->async_handshake(asio::ssl::stream_base::server,
                                               asio::use_awaitable);
        } catch (const std::exception &e) {
          spdlog::error("Websocket failed to handshake with client. reason: {}",
                        e.what());
          continue;
        }

        spdlog::debug("\tSSL Websocket start handling");
        handler_map[id_clone] =
            std::make_unique<handler::WebsocketHandler>(std::move(ssl_socket));
      } else {
        handler_map[id_clone] = std::make_unique<handler::WebsocketHandler>(
            std::move(co_await acceptor.async_accept(asio::use_awaitable)));
      }
      asio::co_spawn(
          co_await asio::this_coro::executor,
          handler_map[id_clone]->handleConnection(),
          [&](const std::exception_ptr e) { handler_map[id_clone].reset(); });
    }
  }

private:
  asio::io_context io_context;
  std::optional<asio::ssl::context> ssl_context;
  asio::ip::tcp::endpoint endpoint;
  asio::ip::tcp::acceptor acceptor;
  std::unordered_map<std::uint64_t, std::unique_ptr<handler::WebsocketHandler>>
      handler_map{};
};
} // namespace ctm::acceptor

#endif