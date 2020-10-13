// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <csignal>
#include <cxxopts.hpp>
#include "cnetwork_asio.h"
#include "config.h"
#include "connection.h"
#include "crash_report.h"
#include "network_thread_pool.h"
#include "packetfactory.h"
#include "platform_defines.h"
#include "version.h"

#include "map_manager.h"
#include "packetfactory.h"

namespace {
void DisplayTitle() {
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if (auto log = console.lock()) {
    log->info( "--------------------------------" );
    log->info( "            osIROSE 2           " );
    log->info( "  http://forum.dev-osrose.com/  " );
    log->info( "--------------------------------" );
    log->info( "Git Branch/Tag/Revision: {}/{}/{}", GIT_BRANCH, GIT_LATEST_TAG, GIT_COMMIT_HASH );
  }
}

void CheckUser() {
#ifndef _WIN32
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if (auto log = console.lock()) {
    if ((getuid() == 0) && (getgid() == 0)) {
      log->warn("You are running as the root superuser.");
      log->warn("It is unnecessary and unsafe to run with root privileges.");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
#endif
}

void ParseCommandLine(int argc, char** argv) {
  cxxopts::Options parser(argv[0], "osIROSE map server");

  try {
    std::string config_file_path = "";
    parser.add_options()
    ("f,config_file",  "Config file path", cxxopts::value<std::string>(config_file_path)
      ->default_value("server.json"), "FILE_PATH")
    ("l,log_level", "Logging level (0-9)", cxxopts::value<int>()
      ->default_value("3"), "LEVEL")
    ("c,core_path", "Path to place minidumps when the app crashes", cxxopts::value<std::string>()
#ifndef _WIN32
      ->default_value("./"), "CORE")
#else
      ->default_value("."), "CORE")
#endif
    ("h,help",  "Print this help text")
    ;

    parser.add_options("Networking")
    ("external_ip", "external IP Address", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "IP")
    ("client_ip", "Client listen IP Address", cxxopts::value<std::string>()
      ->default_value("0.0.0.0"), "IP")
    ("client_port", "Client listen port", cxxopts::value<int>()
      ->default_value("29200"), "PORT")
    ("isc_ip", "ISC listen IP Address", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "IP")
    ("isc_port", "ISC listen port", cxxopts::value<int>()
      ->default_value("29210"), "PORT")
    ("t,max_threads", "Max thread count", cxxopts::value<int>()
      ->default_value("512"), "COUNT")
    ("url", "Auto configure url", cxxopts::value<std::string>()
      ->default_value("http://myexternalip.com/raw"), "URL")
    ;

    parser.add_options("Database")
    ("db_host", "", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "DB_HOST")
    ("db_port", "", cxxopts::value<int>()
      ->default_value("3306"), "DB_PORT")
    ("db_name", "", cxxopts::value<std::string>()
      ->default_value("osirose"), "DB_NAME")
    ("db_user", "", cxxopts::value<std::string>()
      ->default_value("root"), "DB_USER")
    ("db_pass", "", cxxopts::value<std::string>()
      ->default_value(""), "DB_PASS")
    ;

    parser.add_options("Map server")
    ("map_ids", "Map ids this server runs", cxxopts::value<std::vector<uint16_t>>(), "MAP_IDS");

    auto options = parser.parse(argc, argv);

    // Check to see if the user wants to see the help text
    if (options.count("help")) {
      std::cout << parser.help({"", "Database", "Networking"}) << std::endl;
      exit(0);
    }

    Core::Config& config = Core::Config::getInstance(config_file_path);

    // We are using if checks here because we only want to override the config file if the option was supplied
    // Since this is a login server startup function we can get away with a little bit of overhead
    if (options.count("log_level")) config.loginServer().logLevel = options["log_level"].as<int>();

    if (options.count("external_ip")) config.serverData().externalIp = options["external_ip"].as<std::string>();

    if (options.count("client_ip")) config.serverData().listenIp = options["client_ip"].as<std::string>();

    if (options.count("client_port")) config.loginServer().clientPort = options["client_port"].as<int>();

    if (options.count("isc_ip")) config.serverData().iscListenIp = options["isc_ip"].as<std::string>();

    if (options.count("isc_port")) config.loginServer().iscPort = options["isc_port"].as<int>();

    if (options.count("url")) {
      config.serverData().autoConfigureAddress = true;
      config.serverData().autoConfigureUrl = options["url"].as<std::string>();
    }

    if (options.count("max_threads")) {
      config.serverData().maxThreads = options["max_threads"].as<int>();
      Core::NetworkThreadPool::GetInstance(config.serverData().maxThreads);
    }

    if (options.count("core_path")) config.serverData().core_dump_path = options["core_path"].as<std::string>();

    if (options.count("db_host")) config.database().host = options["db_host"].as<std::string>();
    if (options.count("db_port")) config.database().port = options["db_port"].as<int>();
    if (options.count("db_name")) config.database().database = options["db_name"].as<std::string>();
    if (options.count("db_user")) config.database().user = options["db_user"].as<std::string>();
    if (options.count("db_pass")) config.database().password = options["db_pass"].as<std::string>();

    if (options.count("map_ids")) config.mapServer().mapId = options["map_ids"].as<std::vector<uint16_t>>();
  } catch (const cxxopts::OptionException& ex) {
    std::cout << ex.what() << std::endl;
    std::cout << parser.help({"", "Database", "Networking"}) << std::endl;
    exit(1);
  }
}
volatile std::sig_atomic_t gSignalStatus;
}  // namespace

int main(int argc, char* argv[]) {
  try {
    std::signal(SIGINT, [](int signal){ gSignalStatus = signal; });
    std::signal(SIGTERM, [](int signal){ gSignalStatus = signal; });
    RoseCommon::register_recv_packets();
    RoseCommon::register_send_packets();
    ParseCommandLine(argc, argv);

    Core::Config& config = Core::Config::getInstance();
    Core::CrashReport crash_reporter(config.serverData().core_dump_path, "MapServer");
    crash_reporter.set_url(config.serverData().crash_report_url);

    auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
    if (auto log = console.lock()) log->info("Starting up server...");

    Core::CLog::SetLevel((spdlog::level::level_enum)config.mapServer().logLevel);
    DisplayTitle();
    CheckUser();

    if (auto log = console.lock()) {
      log->set_level((spdlog::level::level_enum)config.mapServer().logLevel);
      log->trace("Trace logs are enabled.");
      log->debug("Debug logs are enabled.");
    }
    Core::NetworkThreadPool::GetInstance(config.serverData().maxThreads);
    // if( true == config.serverData().autoConfigureAddress )
    // {
    //   std::string ip_addr = get_current_net_address();
    //   ip_addr.replace(ip_addr.begin(), ip_addr.end(), '\n', '\0');
    //   if(auto log = console.lock()) {
    //     log->info( "Overriding external ip address to \"{}\"", ip_addr );
    //   }
    //   config.serverData().externalIp = ip_addr;
    // }

    Core::connectionPool.addConnector<Core::Osirose>(
          std::bind(Core::mysqlFactory, config.database().user, config.database().password,
                                 config.database().database, config.database().host, config.database().port));

    MapManager app(config.mapServer().mapId);

    while (1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      if(gSignalStatus != 0) {
        app.stop();
        break;
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // we sleep to let all of the other threads time to catch up

    if (auto log = console.lock()) log->info("Server shutting down...");
  } catch (const spdlog::spdlog_ex& ex) {
    std::cout << "Log failed: " << ex.what() << std::endl;
  }
  catch (const sqlpp::exception& ex) {
    std::cout << "Error while trying to access the database: "
              << ex.what() << std::endl;
  }
  return 0;
}
