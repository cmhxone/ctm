#pragma once

#ifndef _CTM_CISCO_SESSION_OPEN_REQ_HPP_
#define _CTM_CISCO_SESSION_OPEN_REQ_HPP_

/*
  OPEN_REQ 패킷 레이아웃
  +------+------------+---------------+
  | MHDR | Fixed Part | Floating Part |
  +------+------------+---------------+
*/

#include "../common/floating_data.hpp"
#include "../common/mhdr.hpp"
#include "../common/serializable.hpp"
#include "../common/tag_value.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <optional>
#include <string_view>
#include <vector>

namespace cisco::session {
class OpenReq {
  public:
    /**
     * @brief Construct a new Open Req object
     *
     */
    OpenReq() {
        common::MHDR mhdr{};
        mhdr.setMessageType(common::MessageType::OPEN_REQ);
        mhdr.setMessageLength(44); // 고정 패킷 길이

        this->mhdr = mhdr;
    }

    /**
     * @brief Destroy the Open Req object
     *
     */
    virtual ~OpenReq() = default;

    /**
     * @brief Get the MHDR object
     *
     * @return const common::MHDR
     */
    const common::MHDR getMHDR() const { return mhdr; }
    /**
     * @brief Get the Invoke ID object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getInvokeID() const { return invoke_id; }
    /**
     * @brief Get the Version Number object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getVersionNumber() const { return version_number; }
    /**
     * @brief Get the Idle Timeout object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getIdleTimeout() const { return idle_timeout; }
    /**
     * @brief Get the Peripheral ID object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getPeripheralID() const { return peripheral_id; }
    /**
     * @brief Get the Services Requested object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getServicesRequested() const {
        return services_requested;
    }
    /**
     * @brief Get the Call Message Mask object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getCallMessageMask() const { return call_message_mask; }
    /**
     * @brief Get the Agent State Mask object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getAgentStateMask() const { return agent_state_mask; }
    /**
     * @brief Get the Config Message Mask object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getConfigMessageMask() const {
        return config_message_mask;
    }
    /**
     * @brief Get the Reserved1 object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getReserved1() const { return reserved_1; }
    /**
     * @brief Get the Reserved2 object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getReserved2() const { return reserved_2; }
    /**
     * @brief Get the Reserved3 object
     *
     * @return const std::uint32_t
     */
    const std::uint32_t getReserved3() const { return reserved_3; }
    /**
     * @brief Get the Client ID object
     *
     * @return const common::FloatingData
     */
    const common::FloatingData getClientID() const { return client_id; }
    /**
     * @brief Get the Client PW object
     *
     * @return const common::FloatingData
     */
    const common::FloatingData getClientPW() const { return client_pw; }
    /**
     * @brief Get the Client Signature object
     *
     * @return const std::optional<common::FloatingData>
     */
    const std::optional<common::FloatingData> getClientSignature() const {
        return client_signature;
    }
    /**
     * @brief Get the Agent Extension object
     *
     * @return const std::optional<common::FloatingData>
     */
    const std::optional<common::FloatingData> getAgentExtension() const {
        return agent_extension;
    }
    /**
     * @brief Get the Agent ID object
     *
     * @return const std::optional<common::FloatingData>
     */
    const std::optional<common::FloatingData> getAgentID() const {
        return agent_id;
    }
    /**
     * @brief Get the Agent Instrument object
     *
     * @return const std::optional<common::FloatingData>
     */
    const std::optional<common::FloatingData> getAgentInstrument() const {
        return agent_instrument;
    }
    /**
     * @brief Get the Application Path ID object
     *
     * @return const std::optional<common::FloatingData>
     */
    const std::optional<common::FloatingData> getApplicationPathID() const {
        return application_path_id;
    }
    /**
     * @brief Get the Unique Instance ID object
     *
     * @return const std::optional<common::FloatingData>
     */
    const std::optional<common::FloatingData> getUniqueInstanceID() const {
        return unique_instance_id;
    }

    /**
     * @brief Set the Invoke ID object
     *
     * @param invoke_id
     */
    void setInvokeID(const std::uint32_t invoke_id) {
        this->invoke_id = invoke_id;
    }
    /**
     * @brief Set the Version Number object
     *
     * @param version_number
     */
    void setVersionNumber(const std::uint32_t version_number) {
        this->version_number = version_number;
    }
    /**
     * @brief Set the Idle Timeout object
     *
     * @param idle_timeout
     */
    void setIdleTimeout(const std::uint32_t idle_timeout) {
        this->idle_timeout = idle_timeout;
    }
    /**
     * @brief Set the Peripheral ID object
     *
     * @param peripheral_id
     */
    void setPeripheralID(const std::uint32_t peripheral_id) {
        this->peripheral_id = peripheral_id;
    }
    /**
     * @brief Set the Services Requested object
     *
     * @param services_requested
     */
    void setServicesRequested(const std::uint32_t services_requested) {
        this->services_requested = services_requested;
    }
    /**
     * @brief Set the Call Message Mask object
     *
     * @param call_message_mask
     */
    void setCallMessageMask(const std::uint32_t call_message_mask) {
        this->call_message_mask = call_message_mask;
    }
    /**
     * @brief Set the Agent State Mask object
     *
     * @param agent_state_mask
     */
    void setAgentStateMask(const std::uint32_t agent_state_mask) {
        this->agent_state_mask = agent_state_mask;
    }
    /**
     * @brief Set the Config Message Mask object
     *
     * @param config_message_mask
     */
    void setConfigMessageMask(const std::uint32_t config_message_mask) {
        this->config_message_mask = config_message_mask;
    }
    /**
     * @brief Set the Reserved1 object
     *
     * @param reserved_1
     */
    void setReserved1(const std::uint32_t reserved_1) {
        this->reserved_1 = reserved_1;
    }
    /**
     * @brief Set the Reserved2 object
     *
     * @param reserved_2
     */
    void setReserved2(const std::uint32_t reserved_2) {
        this->reserved_2 = reserved_2;
    }
    /**
     * @brief Set the Reserved3 object
     *
     * @param reserved_3
     */
    void setReserved3(const std::uint32_t reserved_3) {
        this->reserved_3 = reserved_3;
    }
    /**
     * @brief Set the Client ID object
     *
     * @param client_id
     */
    void setClientID(const std::string_view &client_id) {
        common::FloatingData floating_data{};
        floating_data.setTag(common::TagValue::CLIENT_ID_TAG);
        floating_data.setData(client_id);

        this->mhdr.setMessageLength(this->mhdr.getMessageLength() +
                                    client_id.length() + 4);
        this->client_id = floating_data;
    }
    /**
     * @brief Set the Client PW object
     *
     * @param client_pw
     */
    void setClientPW(const std::string_view &client_pw) {
        common::FloatingData floating_data{};
        floating_data.setTag(common::TagValue::CLIENT_PASSWORD_TAG);
        floating_data.setData(client_pw);

        this->mhdr.setMessageLength(this->mhdr.getMessageLength() +
                                    client_pw.length() + 4);
        this->client_pw = floating_data;
    }
    /**
     * @brief Set the Agent Extension object
     *
     * @param agent_extension
     */
    void setAgentExtension(const common::FloatingData &agent_extension) {
        this->agent_extension = agent_extension;
    }
    /**
     * @brief Set the Agent ID object
     *
     * @param agent_id
     */
    void setAgentID(const common::FloatingData &agent_id) {
        this->agent_id = agent_id;
    }
    /**
     * @brief Set the Agent Instrument object
     *
     * @param agent_instrument
     */
    void setAgentInstrument(const common::FloatingData &agent_instrument) {
        this->agent_instrument = agent_instrument;
    }
    /**
     * @brief Set the Application Path ID object
     *
     * @param application_path_id
     */
    void setApplicationPathID(const common::FloatingData &application_path_id) {
        this->application_path_id = application_path_id;
    }
    /**
     * @brief Set the Unique Instance ID object
     *
     * @param unique_instance_id
     */
    void setUniqueInstanceID(const common::FloatingData &unique_instance_id) {
        this->unique_instance_id = unique_instance_id;
    }

  protected:
  private:
    common::MHDR mhdr;
    std::uint32_t invoke_id;
    std::uint32_t version_number;
    std::uint32_t idle_timeout;
    std::uint32_t peripheral_id = 0xFFFF'FFFF;
    std::uint32_t services_requested;
    std::uint32_t call_message_mask;
    std::uint32_t agent_state_mask;
    std::uint32_t config_message_mask;
    std::uint32_t reserved_1 = 0;
    std::uint32_t reserved_2 = 0;
    std::uint32_t reserved_3 = 0;
    common::FloatingData client_id;
    common::FloatingData client_pw;
    std::optional<common::FloatingData> client_signature;
    std::optional<common::FloatingData> agent_extension;
    std::optional<common::FloatingData> agent_id;
    std::optional<common::FloatingData> agent_instrument;
    std::optional<common::FloatingData> application_path_id;
    std::optional<common::FloatingData> unique_instance_id;
};
} // namespace cisco::session

template <>
inline const std::vector<std::byte>
cisco::common::serialize(const cisco::session::OpenReq &open_req) {
    std::vector<std::byte> result{};

    std::vector<std::vector<std::byte>> members{};
    members.emplace_back(serialize(open_req.getMHDR()));
    members.emplace_back(serialize(open_req.getInvokeID()));
    members.emplace_back(serialize(open_req.getVersionNumber()));
    members.emplace_back(serialize(open_req.getIdleTimeout()));
    members.emplace_back(serialize(open_req.getPeripheralID()));
    members.emplace_back(serialize(open_req.getServicesRequested()));
    members.emplace_back(serialize(open_req.getCallMessageMask()));
    members.emplace_back(serialize(open_req.getAgentStateMask()));
    members.emplace_back(serialize(open_req.getConfigMessageMask()));
    members.emplace_back(serialize(open_req.getReserved1()));
    members.emplace_back(serialize(open_req.getReserved2()));
    members.emplace_back(serialize(open_req.getReserved3()));
    members.emplace_back(serialize(open_req.getClientID()));
    members.emplace_back(serialize(open_req.getClientPW()));
    members.emplace_back(serialize(open_req.getClientSignature()));
    members.emplace_back(serialize(open_req.getAgentExtension()));
    members.emplace_back(serialize(open_req.getAgentID()));
    members.emplace_back(serialize(open_req.getAgentInstrument()));
    members.emplace_back(serialize(open_req.getApplicationPathID()));
    members.emplace_back(serialize(open_req.getUniqueInstanceID()));

    for (const auto &member : members) {
        std::move(member.cbegin(), member.cend(), std::back_inserter(result));
    }

    return result;
}

#endif
