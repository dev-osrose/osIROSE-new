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

#include <cxxopts.hpp>
#include "crash_report.h"
#include "ccharserver.h"
#include "ccharisc.h"
#include "config.h"
#include "version.h"
#include "connection.h"
#include "network_thread_pool.h"
#include "cnetwork_asio.h"
#include "ccharclient.h"

namespace {
void DisplayTitle()
{
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
  if(auto log = console.lock())
  {
    log->info( "--------------------------------" );
    log->info( "        osIROSE 2 Alpha         " );
    log->info( "  http://forum.dev-osrose.com/  " );
    log->info( "--------------------------------" );
    log->info( "Git Branch/Revision: {}/{}", GIT_BRANCH, GIT_COMMIT_HASH );
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
  cxxopts::Options options(argv[0], "osIROSE char server");

  try {
    std::string config_file_path = "";
    options.add_options()
    ("f,file",  "Config file path", cxxopts::value<std::string>(config_file_path)
      ->default_value("server.json"), "FILE_PATH")
    ("l,level", "Logging level (0-9)", cxxopts::value<int>()
      ->default_value("3"), "LEVEL")
    ("ip", "Client listen IP Address", cxxopts::value<std::string>()
      ->default_value("0.0.0.0"), "IP")
    ("port", "Client listen port", cxxopts::value<int>()
      ->default_value("29000"), "PORT")
    ("iscip", "ISC listen IP Address", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "IP")
    ("iscport", "ISC listen port", cxxopts::value<int>()
      ->default_value("29010"), "PORT")
    ("t,maxthreads", "Max thread count", cxxopts::value<int>()
      ->default_value("512"), "COUNT")
    ("h,help",  "Print this help text")
    ;

    options.parse(argc, argv);

    // Check to see if the user wants to see the help text
    if (options.count("help"))
    {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    Core::Config& config = Core::Config::getInstance(config_file_path);

    // We are using if checks here because we only want to override the config file if the option was supplied
    // Since this is a login server startup function we can get away with a little bit of overhead
    if( options.count("level") )
      config.charServer().logLevel = options["level"].as<int>();

    if( options.count("ip") )
      config.serverData().ip = options["ip"].as<std::string>();

    if( options.count("port") )
      config.charServer().clientPort = options["port"].as<int>();

    if( options.count("iscip") )
      config.serverData().iscListenIp = options["iscip"].as<std::string>();

    if( options.count("iscport") )
      config.charServer().iscPort = options["iscport"].as<int>();

    if( options.count("maxthreads") )
      config.serverData().maxThreads = options["maxthreads"].as<int>();
  }
  catch (const cxxopts::OptionException& ex) {
    std::cout << ex.what() << std::endl;
    std::cout << options.help({"", "Group"}) << std::endl;
    exit(1);
  }
}
}

int main(int argc, char* argv[]) {
  try {
  ParseCommandLine(argc, argv);
  Core::CrashReport("/tmp/dumps");
  
  auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);

  if(auto log = console.lock())
    log->info("Starting up server...");

  Core::Config& config = Core::Config::getInstance();
  Core::CLog::SetLevel(static_cast<spdlog::level::level_enum>(config.charServer().logLevel));
  DisplayTitle();
  CheckUser();

  if(auto log = console.lock()) {
    log->set_level(static_cast<spdlog::level::level_enum>(config.charServer().logLevel));
    log->trace("Trace logs are enabled.");
    log->debug("Debug logs are enabled.");
  }
  Core::NetworkThreadPool::GetInstance(config.serverData().maxThreads);

  Core::connectionPool.addConnector(Core::osirose, std::bind(
            Core::mysqlFactory,
            config.database().user,
            config.database().password,
            config.database().database,
            config.database().host,
            0));

  CCharServer clientServer;
  CCharServer iscServer(true);
  CCharISC* iscClient = new CCharISC(std::make_unique<Core::CNetwork_Asio>());
  iscClient->init(config.charServer().loginIp, config.loginServer().iscPort);
  iscClient->SetLogin(true);
  iscClient->connect();
  iscClient->start_recv();

  clientServer.init(config.serverData().ip, config.charServer().clientPort);
  clientServer.listen();
  clientServer.GetISCList().push_front(std::unique_ptr<RoseCommon::CRoseClient>(iscClient));

  iscServer.init(config.serverData().iscListenIp, config.charServer().iscPort);
  iscServer.listen();

  while (clientServer.is_active()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //updateSessions();
  }
  if(auto log = console.lock())
    log->info( "Server shutting down..." );
  Core::NetworkThreadPool::DeleteInstance();
  spdlog::drop_all();
  }
  catch (const spdlog::spdlog_ex& ex) {
     std::cout << "Log failed: " << ex.what() << std::endl;
  }
  return 0;
}
