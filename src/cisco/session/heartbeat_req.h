#pragma once

#ifndef _CTM_CISCO_SESSION_HEARTBEAT_REQ_H_
#define _CTM_CISCO_SESSION_HEARTBEAT_REQ_H_

/*
  HEARTBEAT_REQ 패킷 레이아웃
  +------+------------+
  | MHDR | Fixed Part |
  +------+------------+
*/

#include "../common/mhdr.hpp"
#include "../common/serializable.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <vector>

namespace cisco::session {
class HeartbeatReq {
  public:
    /**
     * @brief Construct a new Heartbeat Req object
     *
     */
    HeartbeatReq() {
        common::MHDR mhdr{};

        mhdr.setMessageType(common::MessageType::HEARTBEAT_REQ);
        mhdr.setMessageLength(4);

        this->mhdr = mhdr;
    }
    /**
     * @brief Destroy the Heartbeat Req object
     *
     */
    virtual ~HeartbeatReq() = default;

    /**
     * @brief
     *
     * @return const common::MHDR
     */
    const common::MHDR getMHDR() const { return this->mhdr; }
    /**
     * @brief Get the Invoke ID object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getInvokeID() const { return this->invoke_id; }

    /**
     * @brief Set the Invoke ID object
     *
     * @param invoke_id
     */
    void setInvokeID(const std::uint32_t invoke_id) {
        this->invoke_id = invoke_id;
    }

  protected:
  private:
    common::MHDR mhdr;
    std::uint32_t invoke_id;
};
} // namespace cisco::session

template <>
inline const std::vector<std::byte>
cisco::common::serialize(const cisco::session::HeartbeatReq &heartbeat_req) {
    std::vector<std::byte> result{};

    const auto mhdr = serialize(heartbeat_req.getMHDR());
    const auto invoke_id = serialize(heartbeat_req.getInvokeID());

    std::move(mhdr.cbegin(), mhdr.cend(), std::back_inserter(result));
    std::move(invoke_id.cbegin(), invoke_id.cend(), std::back_inserter(result));

    return result;
}

template <>
inline const cisco::session::HeartbeatReq
cisco::common::deserialize(const std::vector<std::byte> &bytes) {
    return cisco::session::HeartbeatReq{};
}

#endif