#pragma once

#ifndef _CTM_CTM_BRIDGE_MESSAGE_BRIDGE_HPP_
#define _CTM_CTM_BRIDGE_MESSAGE_BRIDGE_HPP_

#include "../../channel/event/bridge_event.hpp"
#include "../../channel/event/client_event.hpp"
#include "../../channel/event/cti_event.hpp"
#include "../../channel/event/event.hpp"
#include "../../channel/event_channel.hpp"
#include "../../channel/subscriber.hpp"
#include "../../cisco/control/query_agent_state_conf.hpp"
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
      spdlog::debug("MessageBridge received CTI Event");
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

        // 클라이언트에게 메시지 배포
        channel::EventChannel<channel::event::BridgeEvent>::getInstance()
            ->publish(channel::event::BridgeEvent{
                channel::event::BridgeEvent::BridgeEventDestination::CLIENT});
      } break;
      case cisco::common::MessageType::AGENT_STATE_EVENT: {
        const cisco::message::AgentStateEvent agent_state_event =
            cisco::common::deserialize<cisco::message::AgentStateEvent>(
                cti_event->getPacket());
        spdlog::debug(
            "Agent state event received. agent_state: {}, "
            "event_reason_code: {}, icm_agent_id: {}, agent_id: {}, "
            "agent_extension: {}, skill_group_id: {}, "
            "skill_Group_number: {}, state_duration: {}, direction: {}, "
            "mrd_id: {}, peripheral_id: {}",
            agent_state_event.getAgentState(),
            agent_state_event.getEventReasonCode(),
            agent_state_event.getICMAgentID(), agent_state_event.getAgentID(),
            agent_state_event.getAgentExtension(),
            agent_state_event.getSkillGroupID(),
            agent_state_event.getSkillGroupNumber(),
            agent_state_event.getStateDuration(),
            agent_state_event.getDirection(), agent_state_event.getMRDID(),
            agent_state_event.getPeripheralID());

        // 클라이언트에게 메시지 배포
        channel::EventChannel<channel::event::BridgeEvent>::getInstance()
            ->publish(channel::event::BridgeEvent{
                channel::event::BridgeEvent::BridgeEventDestination::CLIENT});
      } break;
      case cisco::common::MessageType::QUERY_AGENT_STATE_CONF: {
        const cisco::control::QueryAgentStateConf query_agent_state_conf =
            cisco::common::deserialize<cisco::control::QueryAgentStateConf>(
                cti_event->getPacket());
        spdlog::debug("Query agent state conf. agent_id: {}, agent_state: {}, "
                      "agent_extension: {}, skill_group_id: {}, "
                      "skill_group_number: {}, icm_agent_id: {}",
                      query_agent_state_conf.getAgentID(),
                      query_agent_state_conf.getAgentState(),
                      query_agent_state_conf.getAgentExtension(),
                      query_agent_state_conf.getSkillGroupID(),
                      query_agent_state_conf.getSkillGroupNumber(),
                      query_agent_state_conf.getICMAgentID());

        // 클라이언트에게 메시지 배포
        channel::EventChannel<channel::event::BridgeEvent>::getInstance()
            ->publish(channel::event::BridgeEvent{
                channel::event::BridgeEvent::BridgeEventDestination::CLIENT});
      } break;
      default:
        spdlog::debug(
            "CTI_Event received. (non-handled message type) message_type: {}",
            static_cast<std::int32_t>(cti_event->getMessageType()));
        break;
      }
    } break;
      // 클라이언트 메시지는 파싱하여 CTI 서버에 던져준다
    case channel::event::EventType::CLIENT_EVENT: {
      spdlog::debug("MessageBridge received Client Event");

      const channel::event::ClientEvent *client_event =
          dynamic_cast<const channel::event::ClientEvent *>(event);

      channel::EventChannel<channel::event::BridgeEvent>::getInstance()
          ->publish(channel::event::BridgeEvent{
              channel::event::BridgeEvent::BridgeEventDestination::CTI});
    } break;
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