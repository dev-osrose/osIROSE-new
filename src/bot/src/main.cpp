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
        LoginClient(std::unique_ptr<Core::INetwork> sock) : CRoseSocket(std::move(sock)) {
            socket_->registerOnConnected(std::bind(&LoginClient::OnConnected, this));
            socket_->registerOnSend(std::bind(&LoginClient::OnSend, this, std::placeholders::_1));
            socket_->registerOnReceived(std::bind(&LoginClient::OnReceived, this, std::placeholders::_1, std::placeholders::_2));
        }
        virtual ~LoginClient() = default;

    protected:
        virtual void OnConnected() {
            logger_->trace("OnConnected");
            auto packet = CliAcceptReq();
            send(packet);
        }

        virtual bool OnSend(uint8_t *buffer) {
            logger_->trace("OnSend");
            (void)buffer;
#ifndef DISABLE_CRYPT
            crypt_.encodeClientPacket(buffer);
#endif
            return true;
        }

        virtual bool OnReceived(uint16_t& packetSize, uint8_t *buffer) {
            logger_->trace("OnReceived");
            if (packetSize == 6) {
#ifndef DISABLE_CRYPT
            packetSize = crypt_.decodeServerHeader(reinterpret_cast<unsigned char*>(buffer));
#else
            packetSize = buffer[0];
#endif
            }
            if (packetSize < 6 || packetSize > MAX_PACKET_SIZE) {
                logger_->debug("Server sent incorrect block header");
                socket_->reset_internal_buffer();
                return false;
            }
#ifndef DISABLE_CRYPT
            if (!crypt_.decodeServerBody(reinterpret_cast<unsigned char*>(buffer))) {
                logger_->debug("Server sent an illegal block");
                socket_->reset_internal_buffer();
                return false;
            }
#endif
            logger_->trace("Received a packet on CRoseSocket {2}: Header [{0}, 0x{1:04x}]", CRosePacket::size(buffer), (uint16_t)CRosePacket::type(buffer), get_id());
#ifdef SPDLOG_TRACE_ON
            fmt::MemoryWriter out;
            for (int i = 0; i < CRosePacket::size(buffer); ++i)
                out.write("0x{0:02x} ", buffer[i]);
            logger_->trace("{}", out.c_str());
#endif
            auto res = std::make_unique<uint8_t[]>(CRosePacket::size(buffer));
            std::memcpy(res.get(), buffer, CRosePacket::size(buffer));
            recv_mutex_.lock();
            recv_queue_.push(std::move(res));
            recv_mutex_.unlock();
            socket_->dispatch([this]() {
                    if (socket_->is_active()) {
                        recv_mutex_.lock();
                        if (!recv_queue_.empty()) {
                            std::unique_ptr<uint8_t[]> buffer = std::move(recv_queue_.front());
                            recv_queue_.pop();
                            if (!HandlePacket(buffer.get())) {
                                logger_->debug("HandlePacket returned false, disconnecting server");
                                socket_->shutdown();
                            }
                        }
                        recv_mutex_.unlock();
                    }
                    });
            socket_->reset_internal_buffer();
            return true;
        }

        virtual bool HandlePacket(uint8_t *buffer) override {
            logger_->trace("Client::HandlePacket start");
            switch (CRosePacket::type(buffer)) {
                case ePacketType::PAKSS_ACCEPT_REPLY:
                    logger_->info("Got accept reply");
                    logger_->info("Trying to login");
                    {
                        auto packet = CliLoginReq("cc03e747a6afbbcbf8be7668acfebee5", "test2");
                        send(packet);
                    }
                    break;
                default:
                    logger_->info("Received a packet : {}", (uint16_t)CRosePacket::type(buffer));
                    break;
            }
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
    loginClient.start_recv();
    while (loginClient.is_active())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    log->info( "Client shutting down..." );
    spdlog::drop_all();
  }
  catch (const spdlog::spdlog_ex& ex) {
     std::cout << "Log failed: " << ex.what() << std::endl;
  }
  return 0;
}
