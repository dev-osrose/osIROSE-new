// #undef UNICODE
//
#include <sstream>
#include "logconsole.h"

std::ostream& Core::Color::operator<<(std::ostream& os,
                                      Core::Color::CodeFG code) {
#ifndef _WIN32
  return os << "\033[1;" << static_cast<int>(code) << "m";
#else
  return os;
#endif
}

std::ostream& Core::Color::operator<<(std::ostream& os,
                                      Core::Color::CodeBG code) {
#ifndef _WIN32
  return os << "\033[" << static_cast<int>(code) << "m";
#else
  return os;
#endif
}

namespace Core {

spdlog::level::level_enum CLog::level_ = spdlog::level::notice;

void CLog::SetLevel(spdlog::level::level_enum _level) {
  level_ = _level;

  std::ostringstream format;
  format << "[%H:%M:%S.%e %z] [%L]";

  if (level_ <= spdlog::level::debug) format << " [thread %t]";
  format << " [%n]" << " %v ";
  spdlog::set_pattern(format.str());
}

std::weak_ptr<spdlog::logger> CLog::GetLogger(
    log_type _type) {
  std::weak_ptr<spdlog::logger> logger;
  try
  {
    switch (_type) {
      case log_type::NETWORK:
        logger = spdlog::get("net");
        break;

      case log_type::DATABASE:
        logger = spdlog::get("db");
        break;

      case log_type::GENERAL:
      default:
        logger = spdlog::get("server");
        break;
    }

    if (logger.expired()) {
      std::ostringstream format;
      format << "[%H:%M:%S.%e %z] [%L]";

      if (level_ <= spdlog::level::debug) format << " [thread %t]";
      format << " [%n]" << " %v ";

      size_t q_size = 1048576;
      spdlog::set_async_mode(q_size, spdlog::async_overflow_policy::discard_log_msg,
                             nullptr, std::chrono::seconds(30));

      std::string path, name;

      switch (_type) {
        case log_type::NETWORK: {
          path = "logs/network";
          name = "net";
          break;
        }
        case log_type::DATABASE: {
          path = "logs/database";
          name = "db";
          break;
        }
        case log_type::GENERAL:
        default: {
          path = "logs/server";
          name = "server";
          break;
        }
      }

      std::vector<spdlog::sink_ptr> net_sink;
      auto console_out = spdlog::sinks::stderr_sink_mt::instance();
      auto console_sink = std::make_shared<spdlog::sinks::ansicolor_sink>(console_out);
      auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
          path.c_str(), "txt", 23, 59);
      net_sink.push_back(console_sink);
      net_sink.push_back(daily_sink);

      auto net_logger = std::make_shared<spdlog::logger>(
          name.c_str(), begin(net_sink), end(net_sink));
      net_logger->set_level(level_);
      net_logger->set_pattern(format.str());

      spdlog::register_logger(net_logger);

      net_sink.clear();
      console_sink.reset();
      daily_sink.reset();
      return net_logger;
    }
  } catch (const spdlog::spdlog_ex& ex) {
    std::cout << "Log failed: " << ex.what() << std::endl;
  }
  return logger;
}
}
