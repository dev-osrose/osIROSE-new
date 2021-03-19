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

#include <curl/curl.h>
#include <cxxopts.hpp>
#include <chrono>

#include "crash_report.h"
#include "nodeserver.h"
#include "nodesessions.h"
#include "config.h"
#include "logconsole.h"
#include "version.h"
#include "network_thread_pool.h"

#include "connection.h"

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

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = reinterpret_cast<char*>(realloc(mem->memory, mem->size + realsize + 1));
  if(mem->memory == NULL) {
    // out of memory!
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

std::string get_current_net_address()
{
  CURL* curl;
  CURLcode res;
  std::string address = "";

  struct MemoryStruct chunk;

  chunk.memory = reinterpret_cast<char*>(malloc(1));  // will be grown as needed by the realloc above
  chunk.size = 0;    // no data at this point

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();

  if(curl)
  {
    Core::Config& config = Core::Config::getInstance();
    curl_easy_setopt(curl, CURLOPT_URL, config.serverData().autoConfigureUrl.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "osirose-node-server/1.0");
    auto cookie = fmt::format("port={};",config.loginServer().clientPort);
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());

    res = curl_easy_perform(curl);

    // Check for errors
    if(res == CURLE_OK)
      address = chunk.memory;
    else
    {
      if(auto log = Core::CLog::GetLogger(Core::log_type::GENERAL).lock())
        log->info( "curl_easy_perform() failed: {}", curl_easy_strerror(res));
    }

    // always cleanup
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  free(chunk.memory);

  return address;
}

void ParseCommandLine(int argc, char** argv)
{
  cxxopts::Options parser(argv[0], "osIROSE node server");

  try {
    std::string config_file_path = "";
    parser.add_options()
    ("f,config_file",  "Config file path", cxxopts::value<std::string>(config_file_path)
      ->default_value("server.json"), "FILE_PATH")
    ("l,log_level", "Logging level (0-9)", cxxopts::value<int>()
      ->default_value("3"), "LEVEL")
    ("c,core_path", "Path to place minidumps when the app crashes", cxxopts::value<std::string>()
#ifndef _WIN32
      ->default_value("/tmp"), "CORE")
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
      ->default_value("29000"), "PORT")
    ("isc_ip", "ISC listen IP Address", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "IP")
    ("isc_port", "ISC listen port", cxxopts::value<int>()
      ->default_value("29010"), "PORT")
    ("t,max_threads", "Max thread count", cxxopts::value<int>()
      ->default_value("512"), "COUNT")
    ("url", "Auto configure url", cxxopts::value<std::string>()
      ->default_value("http://ipv4.myexternalip.com/raw"), "URL")
    ("login_ip", "Login Server's ip address to contact for client auth", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "LOGIN_IP")
    ("login_port", "Login server port", cxxopts::value<int>()
      ->default_value("29000"), "LOGIN_PORT")
    ;

    auto options = parser.parse(argc, argv);

    // Check to see if the user wants to see the help text
    if (options.count("help"))
    {
      std::cout << parser.help({"", "Database", "Networking"}) << std::endl;
      exit(0);
    }

    Core::Config& config = Core::Config::getInstance(config_file_path);

    // We are using if checks here because we only want to override the config file if the option was supplied
    // Since this is a login server startup function we can get away with a little bit of overhead
    if( options.count("log_level") )
      config.nodeServer().logLevel = options["log_level"].as<int>();

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

    // Node server stoof
    if( options.count("login_ip") )
      config.nodeServer().loginIp = options["login_ip"].as<std::string>();
    if( options.count("login_port") )
      config.nodeServer().loginPort = options["login_port"].as<int>();
  }
  catch (const cxxopts::OptionException& ex) {
    std::cout << ex.what() << std::endl;
    std::cout << parser.help({"", "Database", "Networking"}) << std::endl;
    exit(1);
  }
}
} // end namespace

int main(int argc, char* argv[]) {
  try {
    ParseCommandLine(argc, argv);

    Core::Config& config = Core::Config::getInstance();
    Core::CrashReport crash_reporter(config.serverData().core_dump_path, "NodeServer");
    crash_reporter.set_url(config.serverData().crash_report_url);

    auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
    if(auto log = console.lock())
      log->info( "Starting up server..." );

    Core::CLog::SetLevel((spdlog::level::level_enum)config.nodeServer().logLevel);
    DisplayTitle();
    CheckUser();

    if(auto log = console.lock()) {
      log->set_level((spdlog::level::level_enum)config.nodeServer().logLevel);
      log->trace("Trace logs are enabled.");
      log->debug("Debug logs are enabled.");
    }

    Core::NetworkThreadPool::GetInstance(config.serverData().maxThreads);
    if( true == config.serverData().autoConfigureAddress )
    {
      std::string ip_addr = get_current_net_address();
      if(auto log = console.lock()) {
        log->info( "Overriding external ip address to \"{}\"", ip_addr );
      }
      config.serverData().externalIp = ip_addr;
    }

    sqlpp::sqlite3::connection_config db_config;
    db_config.path_to_database = ":memory:";
    db_config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    if(config.nodeServer().logLevel <= spdlog::level::level_enum::debug)
      db_config.debug = true;

    connectionPoolMem.addConnector<NodeDB>([&db_config]() { return std::make_unique<sqlpp::sqlite3::connection>(db_config); });
    {
      auto conn = connectionPoolMem.getConnection<NodeDB>();
      NodeSessionsTable table{};
      conn->execute(R"(CREATE TABLE sessions (
  		  id int(10) NOT NULL,
  		  name varchar(64) DEFAULT NULL,
        state int(2) NOT NULL DEFAULT 0,
        charip varchar(20) DEFAULT NULL,
        charport int(20) DEFAULT NULL,
        worldip varchar(20) DEFAULT NULL,
        worldport int(20) DEFAULT NULL,
        PRIMARY KEY (`id`)
  		))");

  		// Clear the table everything
  		conn(remove_from(table).unconditionally());
    }

    NodeServer loginServer;
    NodeServer charServer;
    NodeServer mapServer;

    loginServer.init(config.serverData().listenIp, config.loginServer().clientPort);
    loginServer.listen();

    charServer.init(config.serverData().listenIp, config.charServer().clientPort);
    charServer.listen();

    mapServer.init(config.serverData().listenIp, config.mapServer().clientPort);
    mapServer.listen();

    while (loginServer.is_active()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if(auto log = console.lock())
      log->info( "Server shutting down..." );
    Core::NetworkThreadPool::DeleteInstance();
    spdlog::shutdown();
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
