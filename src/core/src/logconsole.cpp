// #undef UNICODE
// 
#include <sstream>
#include "logconsole.h"

std::ostream& Core::Color::operator<<(std::ostream& os, Core::Color::CodeFG code)
{
  return os << "\033[1;" << static_cast<int>(code) << "m";
}

std::ostream& Core::Color::operator<<(std::ostream& os, Core::Color::CodeBG code)
{
  return os << "\033[" << static_cast<int>(code) << "m";
}

namespace Core {
std::shared_ptr<spdlog::logger> CLog::CreateLoggers(log_type _type, spdlog::level::level_enum _level)
{
  try
  {
    std::ostringstream format;
    format << Color::FG_GREEN << "[%H:%M:%S.%e %z] [%L] [thread %t]" << Color::FG_WHITE << " %v " << Color::CL_RESET;
//    spdlog::set_pattern( "\033[1;32m" "[%H:%M:%S.%e %z] [%L] [thread %t]" "\033[1;37m" "%v" "\033[0m" );
    spdlog::set_pattern(format.str());

    size_t q_size = 1048576;
    spdlog::set_async_mode( q_size, spdlog::async_overflow_policy::block_retry,
                       nullptr,
                       std::chrono::seconds(30));

    std::string path, name;

    switch(_type)
    {
      case log_type::NETWORK:
      {
        path = "logs/network";
        name = "net";
        break;
      }
      case log_type::DATABASE:
      {
        path = "logs/database";
        name = "db";
        break;
      }
      case log_type::GENERAL:
      default:
      {
        path = "logs/server";
        name = "server";
        break;
      }
    }


    std::vector<spdlog::sink_ptr> net_sink;
    auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(path.c_str(), "txt", 23, 59);
    net_sink.push_back( console_sink );
    net_sink.push_back( daily_sink );

//    std::cout << "console_sink use_count: " << console_sink.use_count() << "\n";
//    std::cout << "daily_sink use_count: " << console_sink.use_count() << "\n";

    auto net_logger = std::make_shared<spdlog::logger>(name.c_str(), begin(net_sink), end(net_sink));
//    std::cout << "net_logger use_count: " << console_sink.use_count() << "\n";
//    std::cout << "console_sink use_count: " << console_sink.use_count() << "\n";
//    std::cout << "daily_sink use_count: " << console_sink.use_count() << "\n";

    net_logger->set_level(_level);

    spdlog::register_logger(net_logger);

    net_sink.clear();
//    std::cout << "console_sink use_count: " << console_sink.use_count() << "\n";
//    std::cout << "daily_sink use_count: " << console_sink.use_count() << "\n";
    console_sink.reset();
    daily_sink.reset();
//    net_logger.reset();
    return std::move(net_logger);
  }
  catch (const spdlog::spdlog_ex& ex)
  {
      std::cout << "Log failed: " << ex.what() << std::endl;
  }
  return nullptr;
}
}
