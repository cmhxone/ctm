#pragma once

#ifndef _CTM_CTM_CTI_CLIENT_H_
#define _CTM_CTM_CTI_CLIENT_H_

#include <Poco/Net/StreamSocket.h>
#include <Poco/Timespan.h>

#include <string>

namespace ctm {
class CTIClient {
  public:
    /**
     * @brief Construct a new CTIClient object
     *
     */
    CTIClient();
    /**
     * @brief Destroy the CTIClient object
     *
     */
    virtual ~CTIClient() = default;

    const CTIClient &operator=(const CTIClient &) = delete;
    CTIClient(const CTIClient &) = delete;

    /**
     * @brief CTI 서버 접속
     *
     */
    void connect() noexcept;
    /**
     * @brief CTI 서버 접속 해제
     *
     */
    void disconnect() noexcept;

  protected:
  private:
    Poco::Net::StreamSocket client_socket{};
    Poco::Timespan connection_timespan{5'000'000};
    Poco::Timespan heartbeat_timespan{5'000'000};
    std::string cti_server_host;
};
} // namespace ctm

#endif