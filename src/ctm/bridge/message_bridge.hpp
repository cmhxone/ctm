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
#include "../agent_info.hpp"
#include "../agent_info_map.hpp"

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
        spdlog::info("HEARTBEAT_CONF received. invoke_id: {}",
                     heart_beat_conf.getInvokeID());
      } break;
      case cisco::common::MessageType::AGENT_STATE_EVENT: {
        // AGENT_STATE_EVENT 응답
        const cisco::message::AgentStateEvent agent_state_event =
            cisco::common::deserialize<cisco::message::AgentStateEvent>(
                cti_event->getPacket());
        spdlog::info(
            "AGENT_STATE_EVENT received. agent_state: {}, "
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

        // 상담원 맵에 저장
        if (AgentInfoMap::getInstance()->exists(
                agent_state_event.getAgentID())) {
          AgentInfo agent_info{};
          agent_info.setAgentID(agent_state_event.getAgentID());
          agent_info.setAgentState(agent_state_event.getAgentState());
          agent_info.setICMAgentID(agent_state_event.getICMAgentID());
          agent_info.setStateDuration(agent_state_event.getStateDuration());
          agent_info.setDirection(agent_state_event.getDirection());
          agent_info.setExtension(agent_state_event.getAgentExtension());
          agent_info.setReasonCode(agent_state_event.getEventReasonCode());
          agent_info.setSkillGroupID(agent_state_event.getSkillGroupID());

          AgentInfoMap::getInstance()->get().emplace(
              agent_state_event.getAgentID(), agent_info);

          agent_info.broadcast();
        } else {
          AgentInfo &agent_info = AgentInfoMap::getInstance()->get().at(
              agent_state_event.getAgentID());

          agent_info.setAgentID(agent_state_event.getAgentID());
          agent_info.setAgentState(agent_state_event.getAgentState());
          agent_info.setICMAgentID(agent_state_event.getICMAgentID());
          agent_info.setStateDuration(agent_state_event.getStateDuration());
          agent_info.setDirection(agent_state_event.getDirection());
          agent_info.setExtension(agent_state_event.getAgentExtension());
          agent_info.setReasonCode(agent_state_event.getEventReasonCode());
          agent_info.setSkillGroupID(agent_state_event.getSkillGroupID());

          agent_info.broadcast();
        }
      } break;
      case cisco::common::MessageType::QUERY_AGENT_STATE_CONF: {
        // QUERY_AGENT_STATE_CONF 응답
        const cisco::control::QueryAgentStateConf query_agent_state_conf =
            cisco::common::deserialize<cisco::control::QueryAgentStateConf>(
                cti_event->getPacket());
        spdlog::info(
            "QUERY_AGENT_STATE_CONF received. agent_id: {}, agent_state: {}, "
            "agent_extension: {}, skill_group_id: {}, "
            "skill_group_number: {}, icm_agent_id: {}",
            query_agent_state_conf.getAgentID(),
            query_agent_state_conf.getAgentState(),
            query_agent_state_conf.getAgentExtension(),
            query_agent_state_conf.getSkillGroupID(),
            query_agent_state_conf.getSkillGroupNumber(),
            query_agent_state_conf.getICMAgentID());

        // 상담원 맵에 저장
        if (AgentInfoMap::getInstance()->exists(
                query_agent_state_conf.getAgentID())) {
          AgentInfo agent_info{};
          agent_info.setAgentID(query_agent_state_conf.getAgentID());
          agent_info.setAgentState(query_agent_state_conf.getAgentState());
          agent_info.setICMAgentID(query_agent_state_conf.getICMAgentID());
          agent_info.setExtension(query_agent_state_conf.getAgentExtension());
          agent_info.setSkillGroupID(query_agent_state_conf.getSkillGroupID());

          AgentInfoMap::getInstance()->get().emplace(
              query_agent_state_conf.getAgentID(), agent_info);

          agent_info.broadcast();
        } else {
          AgentInfo &agent_info = AgentInfoMap::getInstance()->get().at(
              query_agent_state_conf.getAgentID());

          agent_info.setAgentID(query_agent_state_conf.getAgentID());
          agent_info.setAgentState(query_agent_state_conf.getAgentState());
          agent_info.setICMAgentID(query_agent_state_conf.getICMAgentID());
          agent_info.setExtension(query_agent_state_conf.getAgentExtension());
          agent_info.setSkillGroupID(query_agent_state_conf.getSkillGroupID());

          agent_info.broadcast();
        }
      } break;
      case cisco::common::MessageType::AGENT_TEAM_CONFIG_EVENT: {
        // AGENT_TEAM_CONF_EVENT 응답
        const cisco::supervisor::AgentTeamConfigEvent agent_team_config_event =
            cisco::common::deserialize<cisco::supervisor::AgentTeamConfigEvent>(
                cti_event->getPacket());

        std::ostringstream atc_agent_stream;
        for (const cisco::supervisor::ATCAgent &agent :
             agent_team_config_event.getATCAgentList()) {
          atc_agent_stream << "{agent_id: " << agent.atc_agent_id.data()
                           << ", flag: " << agent.agent_flag
                           << ", state: " << agent.atc_agent_state
                           << ", duration: " << agent.atc_agent_state_duration
                           << "}, ";

          // CTI 에게 메시지 배포 (peripheralid-agentid)
          std::ostringstream bridge_message_stream{};
          bridge_message_stream << agent_team_config_event.getPeripheralID()
                                << "-" << agent.atc_agent_id.data();
          bridge_message_stream.flush();

          std::vector<std::byte> bridge_message{};
          for (const char ch : bridge_message_stream.str()) {
            bridge_message.emplace_back(static_cast<std::byte>(ch));
          }

          channel::EventChannel<channel::event::BridgeEvent>::getInstance()
              ->publish(channel::event::BridgeEvent{
                  channel::event::BridgeEvent::BridgeEventDestination::CTI,
                  channel::event::BridgeEvent::BridgeEventMessage{
                      .type = channel::event::BridgeEvent::BridgeEventType::
                          QUERY_AGENT,
                      .message = bridge_message}});

          // 상담원 맵에 저장
          if (AgentInfoMap::getInstance()->exists(agent.atc_agent_id.data())) {
            AgentInfo agent_info{};
            agent_info.setAgentID(agent.atc_agent_id.data());
            agent_info.setAgentState(agent.atc_agent_state);
            agent_info.setStateDuration(agent.atc_agent_state_duration);

            AgentInfoMap::getInstance()->get().emplace(
                agent.atc_agent_id.data(), agent_info);

            agent_info.broadcast();
          } else {
            AgentInfo &agent_info = AgentInfoMap::getInstance()->get().at(
                agent.atc_agent_id.data());

            agent_info.setAgentID(agent.atc_agent_id.data());
            agent_info.setAgentState(agent.atc_agent_state);
            agent_info.setStateDuration(agent.atc_agent_state_duration);

            agent_info.broadcast();
          }
        }

        spdlog::info(
            "AGENT_TEAM_CONF received. peripheral_id: {}, team_id: {}, "
            "number_of_agent: {}, config_operation: {}, department_id: {}, "
            "agent_team_name: {}, atc_agent_list: [{}]",
            agent_team_config_event.getPeripheralID(),
            agent_team_config_event.getTeamID(),
            agent_team_config_event.getNumberOfAgent(),
            agent_team_config_event.getConfigOperation(),
            agent_team_config_event.getDepartmentID(),
            agent_team_config_event.getAgentTeamName(), atc_agent_stream.str());
      } break;
      case cisco::common::MessageType::SYSTEM_EVENT: {
        // SYSTEM_EVENT 응답
        const cisco::misc::SystemEvent system_event =
            cisco::common::deserialize<cisco::misc::SystemEvent>(
                cti_event->getPacket());

        spdlog::info(
            "SYSTEM_EVENT received. pg_status: {}, "
            "icm_central_controller_time: {}, "
            "system_event_id: {}, system_event_arg_1: {}, "
            "system_event_arg_2: "
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
        spdlog::info(
            "CTI Event received. (non-handled message type) message_type: {}",
            static_cast<std::int32_t>(cti_event->getMessageType()));
        break;
      }
    } break;
      // 클라이언트 메시지는 파싱하여 CTI 서버에 던져준다
    case channel::event::EventType::CLIENT_EVENT: {
      spdlog::debug("MessageBridge received Client Event");

      const channel::event::ClientEvent *client_event =
          dynamic_cast<const channel::event::ClientEvent *>(event);
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