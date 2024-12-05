#pragma once

#ifndef _CTM_CTM_BRIDGE_MESSAGE_BRIDGE_HPP_
#define _CTM_CTM_BRIDGE_MESSAGE_BRIDGE_HPP_

#include "../../channel/event/cti_event.hpp"
#include "../../channel/event/event.hpp"
#include "../../channel/subscriber.hpp"
#include "../../cisco/message/agent_state_event.hpp"
#include "../../cisco/session/heartbeat_conf.hpp"
#include "../../template/singleton.hpp"

#include <spdlog/spdlog.h>

#include <cstdint>

namespace ctm::bridge {

class MessageBridge : public channel::Subscriber,
                      public tmpl::Singleton<MessageBridge> {
public:
  /**
   * @brief Construct a new Message Bridge object
   *
   */
  MessageBridge() {}

  /**
   * @brief Destroy the Message Bridge object
   *
   */
  virtual ~MessageBridge() = default;

  /**
   * @brief 이벤트 핸들러
   *
   * @param event
   */
  virtual void handleEvent(const channel::event::Event *event) override {
    switch (event->getEventType()) {
      // CTI 메시지는 파싱하여 클라이언트들에게 던져준다
    case channel::event::EventType::CTI_EVENT: {
      const channel::event::CTIEvent *cti_event =
          dynamic_cast<const channel::event::CTIEvent *>(event);

      switch (cti_event->getMessageType()) {
      // Heartbeat 응답
      case cisco::common::MessageType::HEARTBEAT_CONF: {
        const cisco::session::HeartbeatConf heart_beat_conf =
            cisco::common::deserialize<cisco::session::HeartbeatConf>(
                cti_event->getPacket());
        spdlog::debug("Heartbeat conf received. invoke_id: {}",
                      heart_beat_conf.getInvokeID());
      } break;
      case cisco::common::MessageType::AGENT_STATE_EVENT: {
        const cisco::message::AgentStateEvent agent_state_event =
            cisco::common::deserialize<cisco::message::AgentStateEvent>(
                cti_event->getPacket());
        spdlog::debug(
            "Agent state event received. agent_state: {}, "
            "event_reason_code: {}, icm_agent_id: {}, agent_id: {}, "
            "agent_extension: {}, skill_group_id: {}, "
            "skill_Group_number: {}, state_duration: {}, direction: {}",
            agent_state_event.getAgentState(),
            agent_state_event.getEventReasonCode(),
            agent_state_event.getICMAgentID(), agent_state_event.getAgentID(),
            agent_state_event.getAgentExtension(),
            agent_state_event.getSkillGroupID(),
            agent_state_event.getSkillGroupNumber(),
            agent_state_event.getStateDuration(),
            agent_state_event.getDirection());
      } break;
      default:
        spdlog::debug(
            "CTI_Event received. (non-handled message type) message_type: {}",
            static_cast<std::int32_t>(cti_event->getMessageType()));
        break;
      }
    } break;
      // 클라이언트 메시지는 파싱하여 CTI 서버에 던져준다
    case channel::event::EventType::CLIENT_EVENT:
      break;
      // CTI, 클라이언트 메시지가 아닌 경우 처리하지 않는다
    default:
      return;
    }
  }

protected:
private:
};
} // namespace ctm::bridge

#endif