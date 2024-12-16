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
#include "../../cisco/miscellaneous/system_event.hpp"
#include "../../cisco/session/heartbeat_conf.hpp"
#include "../../cisco/session/open_conf.hpp"
#include "../../cisco/supervisor/agent_team_config_event.hpp"
#include "../../template/singleton.hpp"
#include "../message/agent_message.hpp"

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
  MessageBridge() {
    channel::EventChannel<channel::event::ClientEvent>::getInstance()
        ->subscribe(this);
    channel::EventChannel<channel::event::CTIEvent>::getInstance()->subscribe(
        this);
  }

  /**
   * @brief Destroy the Message Bridge object
   *
   */
  virtual ~MessageBridge() {
    channel::EventChannel<channel::event::ClientEvent>::getInstance()
        ->unsubscribe(this);
    channel::EventChannel<channel::event::CTIEvent>::getInstance()->unsubscribe(
        this);
  };

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
      case cisco::common::MessageType::OPEN_CONF: {
        const cisco::session::OpenConf open_conf =
            cisco::common::deserialize<cisco::session::OpenConf>(
                cti_event->getPacket());

        spdlog::debug(
            "Open conf event recieved. invoke_id: {}, service_granted: {}, "
            "monitor_id: {}, pg_status: {}, icm_central_controller_time: {}, "
            "peripheral_online: {}, peripheral_type: {}, agent_state: {}, "
            "department_id: {}, session_type: {}, agent_extension: {}, "
            "agent_id: {}, agent_instrument: {}, num_peripherals: {}, "
            "flt_peripheral_id: {}, multiline_agent_control: {}",
            open_conf.getInvokeID(), open_conf.getServiceGranted(),
            open_conf.getMonitorID(), open_conf.getPGStatus(),
            open_conf.getICMCentralControllerTime(),
            open_conf.getPeripheralOnline(), open_conf.getPeripheralType(),
            open_conf.getAgentState(), open_conf.getDepartmentID(),
            open_conf.getSessionType(), open_conf.getAgentExtension(),
            open_conf.getAgentID(), open_conf.getAgentInstrument(),
            open_conf.getNumPeripherals(), open_conf.getFltPeripheralID(),
            open_conf.getMultilineAgentControl());
      } break;
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

        // 에이전트 메시지 생성
        message::AgentMessage agent_message{};
        agent_message.setAgentID(agent_state_event.getAgentID());
        agent_message.setAgentState(agent_state_event.getAgentState());
        agent_message.setExtension(agent_state_event.getAgentExtension());
        agent_message.setStateDuration(agent_state_event.getStateDuration());
        agent_message.setReasonCode(agent_state_event.getEventReasonCode());

        // 클라이언트에게 메시지 배포
        channel::EventChannel<channel::event::BridgeEvent>::getInstance()
            ->publish(channel::event::BridgeEvent{
                channel::event::BridgeEvent::BridgeEventDestination::CLIENT,
                std::make_shared<message::AgentMessage>(agent_message)});
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

        // 에이전트 메시지 생성
        message::AgentMessage agent_message{};
        agent_message.setAgentID(query_agent_state_conf.getAgentID());
        agent_message.setAgentState(query_agent_state_conf.getAgentState());
        agent_message.setExtension(query_agent_state_conf.getAgentExtension());

        // 클라이언트에게 메시지 배포
        channel::EventChannel<channel::event::BridgeEvent>::getInstance()
            ->publish(channel::event::BridgeEvent{
                channel::event::BridgeEvent::BridgeEventDestination::CLIENT,
                std::make_shared<message::AgentMessage>(agent_message)});
      } break;
      case cisco::common::MessageType::AGENT_TEAM_CONFIG_EVENT: {
        const cisco::supervisor::AgentTeamConfigEvent agent_team_config_event =
            cisco::common::deserialize<cisco::supervisor::AgentTeamConfigEvent>(
                cti_event->getPacket());
        spdlog::debug(
            "Agent team config event. peripheral_id: {}, team_id: {}, "
            "number_of_agent: {}, config_operation: {}, department_id: {}, "
            "agent_team_name: {}, atc_agent_id: {}, agent_flag: {}, "
            "atc_agent_state: {}, atc_agent_state_duration: {}",
            agent_team_config_event.getPeripheralID(),
            agent_team_config_event.getTeamID(),
            agent_team_config_event.getNumberOfAgent(),
            agent_team_config_event.getConfigOperation(),
            agent_team_config_event.getDepartmentID(),
            agent_team_config_event.getAgentTeamName(),
            agent_team_config_event.getATCAgentID(),
            agent_team_config_event.getAgentFlag(),
            agent_team_config_event.getATCAgentState(),
            agent_team_config_event.getATCAgentStateDuration());

        // 클라이언트에게 메시지 배포
        channel::EventChannel<channel::event::BridgeEvent>::getInstance()
            ->publish(channel::event::BridgeEvent{
                channel::event::BridgeEvent::BridgeEventDestination::CLIENT});
      } break;
      case cisco::common::MessageType::SYSTEM_EVENT: {
        const cisco::misc::SystemEvent system_event =
            cisco::common::deserialize<cisco::misc::SystemEvent>(
                cti_event->getPacket());

        spdlog::debug(
            "System event. pg_status: {}, icm_central_controller_time: {}, "
            "system_event_id: {}, system_event_arg_1: {}, system_event_arg_2: "
            "{}, system_event_arg_3: {}, event_device_type: {}, text: {}, "
            "event_device_id: {}",
            system_event.getPGStatus(),
            system_event.getICMCentralControllerTime(),
            system_event.getSystemEventID(), system_event.getSystemEventArg1(),
            system_event.getSystemEventArg2(),
            system_event.getSystemEventArg3(),
            system_event.getEventDeviceType(), system_event.getText(),
            system_event.getEventDeviceID());
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
}; // namespace ctm::bridge
} // namespace ctm::bridge

#endif