#include <cxxopts.hpp>
#include "logconsole.h"
#include <string>

#include "packetfactory.h"
#include "crosesocket.h"
#include "cnetwork_asio.h"
#include "network_thread_pool.h"
#include "srv_char_list_reply.h"
#include "cli_accept_req.h"
#include "cli_login_req.h"
#include "srv_login_reply.h"
#include "cli_channel_list_req.h"
#include "srv_channel_list_reply.h"
#include "cli_srv_select_req.h"
#include "srv_srv_select_reply.h"
#include "cli_join_server_req.h"
#include "srv_join_server_reply.h"
#include "cli_char_list_req.h"
#include "cli_select_char_req.h"
#include "srv_switch_server.h"
#include "cli_join_server_req.h"
#include "cli_normal_chat.h"

std::string ip = "127.0.0.1";
uint16_t loginPort = 29000;
uint16_t charPort = 29100;
uint16_t mapPort = 29110;

using namespace RoseCommon;
using namespace RoseCommon::Packet;

class Client : public CRoseSocket {
    public:
    Client(std::unique_ptr<Core::INetwork> sock) : CRoseSocket(std::move(sock)) {
        socket_[SocketType::Client]->registerOnConnected(std::bind(&Client::onConnected, this));
        socket_[SocketType::Client]->registerOnSend(std::bind(&Client::onSend, this, std::placeholders::_1));
        socket_[SocketType::Client]->registerOnReceived(std::bind(&Client::onReceived, this, std::placeholders::_1, std::placeholders::_2));
    }
    virtual ~Client() = default;

    protected:
        virtual void onConnected() {
            logger_->trace("Client::OnConnected");
            auto packet = CliAcceptReq();
            send(packet);
        }

        virtual bool onSend(uint8_t *buffer) {
            logger_->trace("Client::OnSend");
            (void)buffer;
#ifndef DISABLE_CRYPT
            crypt_.encodeClientPacket(buffer);
#endif
            return true;
        }

        virtual bool onReceived(uint16_t& packetSize, uint8_t *buffer) {
            logger_->trace("Client::OnReceived");
            if (packetSize == 6) {
#ifndef DISABLE_CRYPT
                packetSize = crypt_.decodeServerHeader(reinterpret_cast<unsigned char*>(buffer));
#else
                packetSize = buffer[0];
#endif
                if (packetSize < 6 || packetSize > MAX_PACKET_SIZE) {
                    logger_->debug("Server sent incorrect block header");
                    socket_[SocketType::Client]->reset_internal_buffer();
                    return false;
                }

                if (packetSize > 6)
                    return true;
            }
#ifndef DISABLE_CRYPT
            if (!crypt_.decodeServerBody(reinterpret_cast<unsigned char*>(buffer))) {
                logger_->debug("Server sent an illegal block");
                socket_[SocketType::Client]->reset_internal_buffer();
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
            socket_[SocketType::Client]->dispatch([this]() {
                    if (socket_[SocketType::Client]->is_active()) {
                        recv_mutex_.lock();
                        if (!recv_queue_.empty()) {
                            std::unique_ptr<uint8_t[]> buffer = std::move(recv_queue_.front());
                            recv_queue_.pop();
                            if (!handlePacket(buffer.get())) {
                                logger_->debug("HandlePacket returned false, disconnecting server");
                                socket_[SocketType::Client]->shutdown();
                            }
                        }
                        recv_mutex_.unlock();
                    }
                    });
            socket_[SocketType::Client]->reset_internal_buffer();
            return true;
        }
};

class LoginClient : public Client {
    public:
        LoginClient(std::unique_ptr<Core::INetwork> sock) : Client(std::move(sock)) {}
        virtual ~LoginClient() = default;

        uint32_t sessionId() const {
            return sessionId_;
        }

        std::string ip() const {
            return ip_;
        }

        uint16_t port() const {
            return port_;
        }

    private:
        uint32_t sessionId_;
        std::string ip_;
        uint16_t port_;

        virtual bool handlePacket(uint8_t *buffer) override {
            logger_->trace("LoginClient::HandlePacket");
            switch (CRosePacket::type(buffer)) {
                case ePacketType::PAKSS_ACCEPT_REPLY:
                    logger_->info("Got accept reply");
                    logger_->info("Trying to login");
                    {
                        auto packet = CliLoginReq::create(std::string("098f6bcd4621d373cade4e832627b4f6"), "test2");
                        send(packet);
                    }
                    break;
                case ePacketType::PAKLC_LOGIN_REPLY:
                    logger_->info("Got a login reply");
                    {
                        auto reply = SrvLoginReply::create(buffer);
                        logger_->info("Got reply result: {}, right: {}, type: {}", reply.get_result(), reply.get_right(), reply.get_type());
                        if (reply.get_result() != 0)
                            return false;
                        logger_->info("Server list:");
                        for (const auto &server : reply.get_serversInfo())
                            logger_->info("{}({}) : test = {}", server.get_name(), server.get_id(), server.get_test());
                        auto packet = CliChannelListReq::create(reply.get_serversInfo()[0].get_id());
                        send(packet);
                    }
                    break;
                case ePacketType::PAKLC_CHANNEL_LIST_REPLY:
                    logger_->info("Got channel list reply");
                    {
                        auto reply = SrvChannelListReply::create(buffer);
                        logger_->info("Got channels for server: {}", reply.get_id());
                        if (!reply.get_channels().size())
                            return false;
                        logger_->info("Channel list:");
                        for (const auto &channel : reply.get_channels())
                            logger_->info("{}({}): {}-{}, {}%", channel.get_name(),
                                    channel.get_id(), channel.get_lowAge(), channel.get_highAge(), channel.get_capacity());
                        auto packet = CliSrvSelectReq::create(reply.get_id(), reply.get_channels()[0].get_id());
                        send(packet);
                    }
                    break;
                case ePacketType::PAKLC_SRV_SELECT_REPLY:
                    logger_->info("Got server select reply");
                    {
                        auto reply = SrvSrvSelectReply::create(buffer);
                        logger_->info("Got reply result: {}", reply.get_result());
                        if (reply.get_result() != SrvSrvSelectReply::OK)
                            return false;
                        sessionId_ = reply.get_sessionId();
                        ip_ = reply.get_ip();
                        port_ = reply.get_port();
                        return false;
                    }
                default:
                    logger_->info("Received a packet : 0x{0:04x}", (uint16_t)CRosePacket::type(buffer));
                    return false;
            }
            return true;
        }
};

class CharClient : public Client {
    public:
  CharClient(uint32_t sessionId, std::unique_ptr<Core::INetwork> sock) : Client(std::move(sock)), sessionId_(sessionId), isOk_(false) {}
      virtual ~CharClient() = default;

  uint32_t sessionId() const {
    return sessionId_;
  }

  uint16_t port() const {
    return port_;
  }

  uint32_t sessionSeed() const {
    return sessionSeed_;
  }

  const std::string &ip() const {
    return ip_;
  }

  bool isOk() const { return isOk_; }

    private:
        uint32_t sessionId_;
        uint16_t port_;
        uint32_t sessionSeed_;
        std::string ip_;
        bool isOk_;

        virtual bool handlePacket(uint8_t *buffer) override {
            logger_->trace("CharClient::HandlePacket");
            switch (CRosePacket::type(buffer)) {
            case ePacketType::PAKSS_ACCEPT_REPLY:
              logger_->info("asking for joinning server");
              {
                auto packet = CliJoinServerReq::create(sessionId_, std::string("098f6bcd4621d373cade4e832627b4f6"));
                send(packet);
              }
              break;
            case ePacketType::PAKSC_JOIN_SERVER_REPLY:
              logger_->info("Got join server reply:");
              {
                auto reply = SrvJoinServerReply::create(buffer);
                logger_->info("Reply: {}, id: {}, payFlag: {}", (uint8_t)reply.get_result(), reply.get_id(), reply.get_payFlag());
                if (reply.get_result() != SrvJoinServerReply::OK)
                  return false;
                auto packet = CliCharListReq();
                send(packet);
              }
              break;
            case ePacketType::PAKCC_CHAR_LIST_REPLY:
              logger_->info("Got character list");
              {
                auto reply = SrvCharListReply::create(buffer);
                logger_->info("This account has {} characters", reply.get_characters().size());
                logger_->info("Character list:");
                for (const auto &it : reply.get_characters()) {
                  logger_->info("name: {}", it.get_name());
                  logger_->info("level: {}", it.get_level());
                  logger_->info("job: {}", it.get_job());
                  logger_->info("race: {}", it.get_race());
                  logger_->info("face: {}", it.get_face());
                  logger_->info("hair: {}", it.get_hair());
                  logger_->info("platinium: {}", it.get_platinium());
                  logger_->info("seconds until delete: {}", it.get_remainSecsUntilDelete());
                  logger_->info("items:");
                  size_t i = 0;
                  for (const auto& item : it.get_items()) {
                    logger_->info("Item in position {}", i);
                    logger_->info("id: {}", item.get_id());
                    if (item.get_id() == 0)
                      continue;
                    logger_->info("gemOpt: {}", item.get_gem_opt());
                    logger_->info("socket: {}", item.get_socket());
                    logger_->info("grade: {}", item.get_grade());
                    ++i;
                  }
                }
                if (reply.get_characters().size() == 0) {
                    logger_->info("No character!");
                    return false;
                }
                auto packet = CliSelectCharReq::create(0, 0, 0, reply.get_characters()[0].get_name());
                send(packet);
              }
              break;
            case ePacketType::PAKCC_SWITCH_SERVER:
              logger_->info("Got character select reply");
              {
                auto reply = SrvSwitchServer::create(buffer);
                ip_ = reply.get_ip();
                port_ = reply.get_port();
                sessionId_ = reply.get_sessionId();
                sessionSeed_ = reply.get_sessionSeed();
                isOk_ = true;
              }
              return true;
            default:
              logger_->info("Received a packet : 0x{0:04x}", (uint16_t)CRosePacket::type(buffer));
              return false;
            }
            return true;
        }
};

class MapClient : public Client {
public:
  MapClient(uint32_t sessionId, uint32_t sessionSeed, std::unique_ptr<Core::INetwork> sock) : Client(std::move(sock)), sessionId_(sessionId), sessionSeed_(sessionSeed) {}
  virtual ~MapClient() = default;

private:
  uint32_t sessionId_;
  uint32_t sessionSeed_;

  virtual bool handlePacket(uint8_t *buffer) override {
    logger_->trace("MapClient::HandlePacket");
    (void)buffer;
    (void)sessionId_;
    (void)sessionSeed_;
    switch (CRosePacket::type(buffer)) {
    case ePacketType::PAKSS_ACCEPT_REPLY:
      logger_->info("asking for joinning server");
      {
        auto packet = CliJoinServerReq::create(sessionId_, std::string("098f6bcd4621d373cade4e832627b4f6"));
        send(packet);
      }
      break;
    case ePacketType::PAKSC_JOIN_SERVER_REPLY:
      logger_->info("Logged in.");
      break;
    case ePacketType::PAKWC_SELECT_CHAR_REPLY:
      logger_->info("Got char data reply");
      break;
    case ePacketType::PAKWC_INVENTORY_DATA:
      logger_->info("Got inventory data reply");
      break;
    case ePacketType::PAKWC_QUEST_DATA:
      logger_->info("Got quest data reply");
      break;
    case ePacketType::PAKWC_BILLING_MESSAGE:
      logger_->info("Got billing message reply");
      logger_->info("Sending a chat message");
      {
          auto packet = CliNormalChat::create("This is a test message from a bot");
          send(packet);
      }
      break;
    default:
      logger_->info("Received a packet : 0x{0:04x}", (uint16_t)CRosePacket::type(buffer));
      return false;
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
    while (loginClient.is_active())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (loginClient.ip().size()) {
        log->info("Connecting to char server: {}:{}", loginClient.ip(), loginClient.port());
        socket = std::make_unique<Core::CNetwork_Asio>();
        socket->init(loginClient.ip(), loginClient.port());
        CharClient charClient{loginClient.sessionId(), std::move(socket)};
        charClient.connect();
        charClient.start_recv();
        while (charClient.is_active() && !charClient.isOk())
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (charClient.ip().size()) {
          log->info("Connecting to map server: {}:{}", charClient.ip(), charClient.port());
          socket = std::make_unique<Core::CNetwork_Asio>();
          socket->init(charClient.ip(), charClient.port());
          MapClient mapClient{charClient.sessionId(), charClient.sessionSeed(), std::move(socket)};
          mapClient.connect();
          mapClient.start_recv();
          while (mapClient.is_active() && charClient.is_active())
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
