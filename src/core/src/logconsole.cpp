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
std::weak_ptr<spdlog::logger> CLog::GetLogger(
    log_type _type, spdlog::level::level_enum _level) {
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
      format << Color::FG_GREEN << "[%H:%M:%S.%e %z] [%L]";

      if (_level <= spdlog::level::debug) format << " [thread %t]";
      format << " [%n]" << Color::FG_WHITE << " %v " << Color::CL_RESET;

      size_t q_size = 1048576;
      spdlog::set_async_mode(q_size, spdlog::async_overflow_policy::block_retry,
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
      auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
      auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
          path.c_str(), "txt", 23, 59);
      net_sink.push_back(console_sink);
      net_sink.push_back(daily_sink);

      auto net_logger = std::make_shared<spdlog::logger>(
          name.c_str(), begin(net_sink), end(net_sink));
      net_logger->set_level(_level);
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
