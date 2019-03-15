// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <csignal>
#include <chrono>
#include <cxxopts.hpp>
#include "config.h"
#include "version.h"
#include "logconsole.h"
#include "network_thread_pool.h"
#include "connection.h"
#include "mysqlconnection.h"
#include "crash_report.h"

#include "cloginserver.h"

namespace {
void DisplayTitle()
{
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if(auto log = console.lock())
  {
    log->info( "--------------------------------" );
    log->info( "            osIROSE 2           " );
    log->info( "  http://forum.dev-osrose.com/  " );
    log->info( "--------------------------------" );
    log->info( "Git Branch/Tag/Revision: {}/{}/{}", GIT_BRANCH, GIT_LATEST_TAG, GIT_COMMIT_HASH );
  }
}

void CheckUser()
{
#ifndef _WIN32
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if(auto log = console.lock())
  {
    if ((getuid() == 0) && (getgid() == 0)) {
      log->warn( "You are running as the root superuser." );
      log->warn( "It is unnecessary and unsafe to run with root privileges." );
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
#endif
}

void ParseCommandLine(int argc, char** argv)
{
  cxxopts::Options options(argv[0], "osIROSE login server");

  try {
    std::string config_file_path = "";
    options.add_options()
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
    
    options.add_options("Networking")
    ("external_ip", "external IP Address", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "IP")
    ("client_ip", "Client listen IP Address", cxxopts::value<std::string>()
      ->default_value("0.0.0.0"), "IP")
    ("client_port", "Client listen port", cxxopts::value<int>()
      ->default_value("29000"), "PORT")
    ("isc_ip", "ISC listen IP Address", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "IP")
    ("isc_port", "ISC listen port", cxxopts::value<int>()
      ->default_value("29010"), "PORT")
    ("t,max_threads", "Max thread count", cxxopts::value<int>()
      ->default_value("512"), "COUNT")
    ("url", "Auto configure url", cxxopts::value<std::string>()
      ->default_value("http://myexternalip.com/raw"), "URL")
    ;
    
    options.add_options("Database")
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

    options.parse(argc, argv);

    // Check to see if the user wants to see the help text
    if (options.count("help"))
    {
      std::cout << options.help({"", "Database", "Networking"}) << std::endl;
      exit(0);
    }

    Core::Config& config = Core::Config::getInstance(config_file_path);

    // We are using if checks here because we only want to override the config file if the option was supplied
    // Since this is a login server startup function we can get away with a little bit of overhead
    if( options.count("log_level") )
      config.loginServer().logLevel = options["log_level"].as<int>();

    if( options.count("external_ip") )
      config.serverData().externalIp = options["external_ip"].as<std::string>();
      
    if( options.count("client_ip") )
      config.serverData().listenIp = options["client_ip"].as<std::string>();

    if( options.count("client_port") )
      config.loginServer().clientPort = options["client_port"].as<int>();

    if( options.count("isc_ip") )
      config.serverData().iscListenIp = options["isc_ip"].as<std::string>();

    if( options.count("isc_port") )
      config.loginServer().iscPort = options["isc_port"].as<int>();
      
    if( options.count("url") )
    {
      config.serverData().autoConfigureAddress = true;
      config.serverData().autoConfigureUrl = options["url"].as<std::string>();
    }
    
    if( options.count("max_threads") ) 
    {
      config.serverData().maxThreads = options["max_threads"].as<int>();
      Core::NetworkThreadPool::GetInstance(config.serverData().maxThreads);
    }
    
    if( options.count("core_path") )
      config.serverData().core_dump_path = options["core_path"].as<std::string>();
      
    if( options.count("db_host") )
      config.database().host = options["db_host"].as<std::string>();
    if( options.count("db_port") )
      config.database().port = options["db_port"].as<int>();
    if( options.count("db_name") )
      config.database().database = options["db_name"].as<std::string>();
    if( options.count("db_user") )
      config.database().user = options["db_user"].as<std::string>();
    if( options.count("db_pass") )
      config.database().password = options["db_pass"].as<std::string>();
  }
  catch (const cxxopts::OptionException& ex) {
    std::cout << ex.what() << std::endl;
    std::cout << options.help({"", "Database", "Networking"}) << std::endl;
    exit(1);
  }
}

void deleteStaleSessions() {
  using namespace std::chrono_literals;
  using ::date::floor;
  static std::chrono::steady_clock::time_point time{};
  if (Core::Time::GetTickCount() - time < 5min)
    return;
  time = Core::Time::GetTickCount();
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::SessionTable session{};
  Core::AccountTable table{};
  conn(sqlpp::update(table.join(session).on(table.id == session.userid)).set(table.online = 0).where(session.time < floor<std::chrono::minutes>(std::chrono::system_clock::now()) - 5min));
  conn(sqlpp::remove_from(session).where(session.time < floor<std::chrono::minutes>(std::chrono::system_clock::now()) - 5min));
}

volatile std::sig_atomic_t gSignalStatus = 0;

SQLPP_ALIAS_PROVIDER(total);

void delete_stale_parties() {
  using namespace std::chrono_literals;
  using ::date::floor;
  static std::chrono::steady_clock::time_point time{};
  if (Core::Time::GetTickCount() - time < 5min) {
    return;
  }
  time = Core::Time::GetTickCount();
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::PartyTable party{};
  Core::PartyMembersTable party_members{};

  auto party_count = conn(sqlpp::select(party.id,
    sqlpp::count(party_members.id).as(total),
    party_members.memberId)
      .from(party_members.join(party).on(party.id == party_members.id))
      .unconditionally());
  std::vector<int> to_remove;
  std::vector<std::pair<int, int>> members_to_remove;
  for (const auto& row : party_count) {
    if (row.total <= 1) {
      to_remove.push_back(row.id);
    }
    if (static_cast<long int>(row.total) == 1) {
      members_to_remove.emplace_back(row.id, row.memberId);
    }
  }
  for (const auto& p : to_remove) {
    conn(sqlpp::remove_from(party).where(party.id == p));
  }
  for (const auto& member : members_to_remove) {
    conn(sqlpp::remove_from(party_members).where(party_members.id == member.first and party_members.memberId == member.second));
  }
}

} // end namespace

int main(int argc, char* argv[]) {
  try {
    std::signal(SIGINT, [](int signal){ gSignalStatus = signal; });
    std::signal(SIGTERM, [](int signal){ gSignalStatus = signal; });
    ParseCommandLine(argc, argv);
    
    Core::Config& config = Core::Config::getInstance();
    Core::CrashReport crash_reporter(config.serverData().core_dump_path);

    auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
    Core::CLog::SetLevel((spdlog::level::level_enum)config.loginServer().logLevel);
    if(auto log = console.lock()) {
      log->info( "Starting up server..." );
    }
    DisplayTitle();
    CheckUser();

    if(auto log = console.lock()) {
      log->set_level((spdlog::level::level_enum)config.loginServer().logLevel);
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

    Core::connectionPool.addConnector<Core::Osirose>(std::bind(
                Core::mysqlFactory,
                config.database().user,
                config.database().password,
                config.database().database,
                config.database().host,
                config.database().port));

    CLoginServer iscServer(true);
    CLoginServer clientServer(false, &iscServer);

    clientServer.init(config.serverData().listenIp, config.loginServer().clientPort);
    clientServer.listen();

    iscServer.init(config.serverData().iscListenIp, config.loginServer().iscPort);
    iscServer.listen();

    while (clientServer.is_active()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      deleteStaleSessions();
      
      if(gSignalStatus != 0) {
        clientServer.shutdown(true);
        iscServer.shutdown(true);
      }
      delete_stale_parties();
    }

    if(auto log = console.lock())
      log->info( "Server shutting down..." );
    Core::NetworkThreadPool::DeleteInstance();
    spdlog::drop_all();

  }
  catch (const spdlog::spdlog_ex& ex) {
     std::cout << "Log failed: " << ex.what() << std::endl;
  }
  catch (const sqlpp::exception& ex) {
    std::cout << "Error while trying to access the database: "
              << ex.what() << std::endl;
  }
  return 0;
}
