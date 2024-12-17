#pragma once

#ifndef _CTM_CTM_AGENT_INFO_MAP_HPP_
#define _CTM_CTM_AGENT_INFO_MAP_HPP_

#include "../template/singleton.hpp"
#include "./agent_info.hpp"

#include <algorithm>
#include <string_view>
#include <unordered_map>


namespace ctm {
/**
 * @brief 상담원 상태를 저장하는 맵맵
 *
 */
class AgentInfoMap : public tmpl::Singleton<AgentInfoMap> {
public:
  /**
   * @brief Construct a new Agent Info Set object
   *
   */
  AgentInfoMap() {};
  /**
   * @brief Destroy the Agent Info Set object
   *
   */
  virtual ~AgentInfoMap() = default;

  /**
   * @brief 내부 맵을을 가져온다
   *
   * @return std::unordered_map<std::string, AgentInfo>&
   */
  std::unordered_map<std::string, AgentInfo> &get() { return inner_map; }

  /**
   * @brief 해당 상담직원이 이미 맵맵에 있는지 판단
   *
   * @param agent_id
   * @return true
   * @return false
   */
  const bool exists(const std::string_view agent_id) const {
    return std::find_if(inner_map.cbegin(), inner_map.cend(),
                        [&](const std::pair<std::string, AgentInfo> &info) {
                          return info.first == agent_id.data();
                        }) != inner_map.cend();
  }

protected:
private:
  std::unordered_map<std::string, AgentInfo> inner_map{};
};
} // namespace ctm

#endif