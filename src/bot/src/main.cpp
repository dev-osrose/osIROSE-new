#include <cxxopts.hpp>
#include "logconsole.h"
#include <string>

#include "cnetwork_asio.h"
#include "network_thread_pool.h"

#include "login_client.h"
#include "char_client.h"
#include "map_client.h"

std::string ip = "127.0.0.1";
uint16_t loginPort = 29000;
uint16_t charPort = 29100;
uint16_t mapPort = 29110;


void ParseCommandLine(int argc, char** argv)
{
  cxxopts::Options parser(argv[0], "osIROSE bot");
  try {
    std::string config_file_path = "";
    parser.add_options()
    ("ip", "server IP Address", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "IP")
    ("h,help",  "Print this help text")
    ;
    auto options = parser.parse(argc, argv);
    if (options.count("help"))
    {
      std::cout << parser.help({"", "Group"}) << std::endl;
      exit(0);
    }
    ip = options["ip"].as<std::string>();
  }
  catch (const cxxopts::OptionException& ex) {
    std::cout << ex.what() << std::endl;
    std::cout << parser.help({"", "Group"}) << std::endl;
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  try {
    ParseCommandLine(argc, argv);

    Core::CLog::SetLevel((spdlog::level::level_enum)0);
    auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
    auto log = console.lock();
    log->info( "Starting up client..." );

    Core::NetworkThreadPool::GetInstance(1);

    auto socket = std::make_unique<Core::CNetwork_Asio>();
    socket->init(ip, loginPort);
    log->info("Connecting to login server: {}:{}", ip, loginPort);
    LoginClient loginClient{std::move(socket)};
    loginClient.connect();
    loginClient.start_recv();
    while (loginClient.is_active()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (loginClient.ip().size()) {
        log->info("Connecting to char server: {}:{}", loginClient.ip(), loginClient.port());
        socket = std::make_unique<Core::CNetwork_Asio>();
        socket->init(loginClient.ip(), loginClient.port());
        CharClient charClient{loginClient.sessionId(), std::move(socket)};
        charClient.connect();
        charClient.start_recv();
        while (charClient.is_active()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (charClient.ip().size()) {
          log->info("Connecting to map server: {}:{}", charClient.ip(), charClient.port());
          socket = std::make_unique<Core::CNetwork_Asio>();
          socket->init(charClient.ip(), charClient.port());
          MapClient mapClient{charClient.sessionId(), charClient.sessionSeed(), std::move(socket)};
          mapClient.connect();
          mapClient.start_recv();
          while (mapClient.is_active() && charClient.is_active()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
          }
        }
    }

    log->info( "Client shutting down..." );
    spdlog::drop_all();
  }
  catch (const spdlog::spdlog_ex& ex) {
     std::cout << "Log failed: " << ex.what() << std::endl;
  }
  return 0;
}
