#include "./ctm/ctm.h"
#include "./util/ini_loader.h"

#include <memory>
#include <spdlog/common.h>
#include <spdlog/formatter.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <cstdlib>
#include <memory.h>
#include <thread>
#include <vector>

using namespace std;
using namespace channel::event;

int main(int argc, char **argv) {
  // INI 파일에서 로거 설정 추출
  const std::string level_string = util::IniLoader::getInstance()->get(
      "log", "log.level", std::string("info"));
  const std::string log_file_string = util::IniLoader::getInstance()->get(
      "log", "log.file.path", std::string("./log/ctmpp.log"));
  const bool is_stdout =
      util::IniLoader::getInstance()->get("log", "log.stdout.enabled", true);
  const bool is_fileout =
      util::IniLoader::getInstance()->get("log", "log.file.enabled", false);

  // 로그 싱크 생성
  vector<shared_ptr<spdlog::sinks::sink>> log_sinks{};

  // 콘솔 로거 생성
  if (is_stdout) {
    shared_ptr<spdlog::sinks::sink> logger =
        make_shared<spdlog::sinks::stdout_color_sink_mt>();

    log_sinks.emplace_back(std::move(logger));
  }

  // 파일 로거 생성
  if (is_fileout) {
    shared_ptr<spdlog::sinks::sink> logger =
        make_shared<spdlog::sinks::daily_file_sink_mt>(log_file_string, 0, 0);

    log_sinks.emplace_back(std::move(logger));
  }

  // 멀티 싱크 로거 생성
  shared_ptr<spdlog::logger> multi_sink_logger = make_shared<spdlog::logger>(
      "ctmpp", log_sinks.cbegin(), log_sinks.cend());

  // 로그 레벨 설정
  spdlog::level::level_enum log_level{};
  if (level_string == "trace") {
    log_level = spdlog::level::level_enum::trace;
  } else if (level_string == "debug") {
    log_level = spdlog::level::level_enum::debug;
  } else if (level_string == "info") {
    log_level = spdlog::level::level_enum::info;
  } else if (level_string == "warn") {
    log_level = spdlog::level::level_enum::warn;
  } else if (level_string == "error") {
    log_level = spdlog::level::level_enum::err;
  } else if (level_string == "critical") {
    log_level = spdlog::level::level_enum::critical;
  } else {
    log_level = spdlog::level::level_enum::info;
  }
  multi_sink_logger->set_level(log_level);

  // 로그 패턴 설정
  multi_sink_logger->set_pattern("[%5l][%Y-%m-%d %H:%M:%S.%e][%n][%t] - %v");
  multi_sink_logger->set_level(log_level);
  multi_sink_logger->flush_on(log_level);

  // 기본 로그 설정
  spdlog::set_default_logger(multi_sink_logger);

  // CTM 실행
  ctm::CTM ctm{};

  // 프로세스 홀딩
  while (true) {
    this_thread::sleep_for(chrono::milliseconds{100});
  }

  spdlog::debug("Done");
  spdlog::shutdown();
  return EXIT_SUCCESS;
}