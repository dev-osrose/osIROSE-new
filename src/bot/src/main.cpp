#include <cxxopts.hpp>
#include "logconsole.h"
#include <string>
#include "packets.h"
#include "packetfactory.h"
#include "crosesocket.h"
#include "cnetwork_asio.h"

std::string ip = "127.0.0.1";
uint16_t loginPort = 29000;
uint16_t charPort = 29100;
uint16_t mapPort = 29110;

using namespace RoseCommon;

class LoginClient : public CRoseSocket {
    public:
        LoginClient(std::unique_ptr<Core::INetwork> sock) : CRoseSocket(std::move(sock)) {}
        virtual ~LoginClient() = default;

        bool HandlePacket(uint8_t *buffer) override {
            logger_->trace("Client::HandlePacket start");
            (void)buffer;
            return true;
        }
};

void ParseCommandLine(int argc, char** argv)
{
  cxxopts::Options options(argv[0], "osIROSE bot");
  try {
    std::string config_file_path = "";
    options.add_options()
    ("ip", "server IP Address", cxxopts::value<std::string>()
      ->default_value("127.0.0.1"), "IP")
    ("h,help",  "Print this help text")
    ;
    options.parse(argc, argv);
    if (options.count("help"))
    {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }
    ip = options["ip"].as<std::string>();
  }
  catch (const cxxopts::OptionException& ex) {
    std::cout << ex.what() << std::endl;
    std::cout << options.help({"", "Group"}) << std::endl;
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  try {
    ParseCommandLine(argc, argv);

    auto console = Core::CLog::GetLogger(Core::log_type::GENERAL);
    auto log = console.lock();
    log->info( "Starting up client..." );
    log->set_level((spdlog::level::level_enum)0);

    auto socket = std::make_unique<Core::CNetwork_Asio>();
    socket->init(ip, loginPort);
    LoginClient loginClient{std::move(socket)};
    loginClient.connect();
    auto packet = CliLoginReq("cc03e747a6afbbcbf8be7668acfebee5", "test2");
    loginClient.start_recv();
    loginClient.send(packet);
    while (true);

    log->info( "Client shutting down..." );
    spdlog::drop_all();
  }
  catch (const spdlog::spdlog_ex& ex) {
     std::cout << "Log failed: " << ex.what() << std::endl;
  }
  return 0;
}
