#include <cxxopts.hpp>
#include "logconsole.h"
#include <string>
#include "packets.h"
#include "packetfactory.h"
#include "crosesocket.h"
#include "cnetwork_asio.h"
#include "network_thread_pool.h"
#include "charpackets.h"

#define SPDLOG_TRACE_ON

std::string ip = "127.0.0.1";
uint16_t loginPort = 29000;
uint16_t charPort = 29100;
uint16_t mapPort = 29110;

using namespace RoseCommon;

class Client : public CRoseSocket {
    public:
    Client(std::unique_ptr<Core::INetwork> sock) : CRoseSocket(std::move(sock)) {
        socket_->registerOnConnected(std::bind(&Client::OnConnected, this));
        socket_->registerOnSend(std::bind(&Client::OnSend, this, std::placeholders::_1));
        socket_->registerOnReceived(std::bind(&Client::OnReceived, this, std::placeholders::_1, std::placeholders::_2));
    }
    virtual ~Client() = default;

    protected:
        virtual void OnConnected() {
            logger_->trace("LoginClient::OnConnected");
            auto packet = CliAcceptReq();
            send(packet);
        }

        virtual bool OnSend(uint8_t *buffer) {
            logger_->trace("LoginClient::OnSend");
            (void)buffer;
#ifndef DISABLE_CRYPT
            crypt_.encodeClientPacket(buffer);
#endif
            return true;
        }

        virtual bool OnReceived(uint16_t& packetSize, uint8_t *buffer) {
            logger_->trace("LoginClient::OnReceived");
            if (packetSize == 6) {
#ifndef DISABLE_CRYPT
                packetSize = crypt_.decodeServerHeader(reinterpret_cast<unsigned char*>(buffer));
#else
                packetSize = buffer[0];
#endif
                if (packetSize < 6 || packetSize > MAX_PACKET_SIZE) {
                    logger_->debug("Server sent incorrect block header");
                    socket_->reset_internal_buffer();
                    return false;
                }

                if (packetSize > 6)
                    return true;
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

        virtual bool HandlePacket(uint8_t *buffer) override {
            logger_->trace("LoginClient::HandlePacket");
            switch (CRosePacket::type(buffer)) {
                case ePacketType::PAKSS_ACCEPT_REPLY:
                    logger_->info("Got accept reply");
                    logger_->info("Trying to login");
                    {
                        auto packet = CliLoginReq("098f6bcd4621d373cade4e832627b4f6", "test2");
                        send(packet);
                    }
                    break;
                case ePacketType::PAKLC_LOGIN_REPLY:
                    logger_->info("Got a login reply");
                    {
                        auto reply = SrvLoginReply(buffer);
                        logger_->info("Got reply result: {}, right: {}, type: {}", reply.result(), reply.right(), reply.type());
                        if (reply.result() != 0)
                            return false;
                        logger_->info("Server list:");
                        for (const auto &server : reply.servers())
                            logger_->info("{}({}) : {}", server.name_, server.id_, server.test_);
                        auto packet = CliChannelListReq(reply.servers()[0].id_);
                        send(packet);
                    }
                    break;
                case ePacketType::PAKLC_CHANNEL_LIST_REPLY:
                    logger_->info("Got channel list reply");
                    {
                        auto reply = SrvChannelListReply(buffer);
                        logger_->info("Got channels for server: {}", reply.id());
                        if (!reply.channels().size())
                            return false;
                        logger_->info("Channel list:");
                        for (const auto &channel : reply.channels())
                            logger_->info("{}({}): {}-{}, {}%", channel.name_, channel.id_, channel.lowAge_, channel.highAge_, channel.capacity_);
                        auto packet = CliSrvSelectReq(reply.id(), reply.channels()[0].id_);
                        send(packet);
                    }
                    break;
                case ePacketType::PAKLC_SRV_SELECT_REPLY:
                    logger_->info("Got server select reply");
                    {
                        auto reply = SrvSrvSelectReply(buffer);
                        logger_->info("Got reply result: {}", reply.result());
                        if (reply.result() != SrvSrvSelectReply::OK)
                            return false;
                        sessionId_ = reply.sessionId();
                        ip_ = reply.ip();
                        port_ = reply.port();
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
  CharClient(uint32_t sessionId, std::unique_ptr<Core::INetwork> sock) : Client(std::move(sock)), sessionId_(sessionId) {}
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

    private:
        uint32_t sessionId_;
        uint16_t port_;
        uint32_t sessionSeed_;
        std::string ip_;

        virtual bool HandlePacket(uint8_t *buffer) override {
            logger_->trace("CharClient::HandlePacket");
            switch (CRosePacket::type(buffer)) {
            case ePacketType::PAKSS_ACCEPT_REPLY:
              logger_->info("asking for joinning server");
              {
                auto packet = CliJoinServerReq(sessionId_, "098f6bcd4621d373cade4e832627b4f6");
                send(packet);
              }
              break;
            case ePacketType::PAKSC_JOIN_SERVER_REPLY:
              logger_->info("Got join server reply:");
              {
                auto reply = SrvJoinServerReply(buffer);
                logger_->info("Reply: {}, id: {}, payFlag: {}", (uint8_t)reply.result(), reply.id(), reply.payFlag());
                if (reply.result() != SrvJoinServerReply::OK)
                  return false;
                auto packet = CliCharListReq();
                send(packet);
              }
              break;
            case ePacketType::PAKCC_CHAR_LIST_REPLY:
              logger_->info("Got character list");
              {
                auto reply = SrvCharacterListReply(buffer);
                logger_->info("This account has {} characters", reply.getCharacterCount());
                logger_->info("Character list:");
                for (const auto &it : reply.getCharacters()) {
                  logger_->info("name: {}", it.name_);
                  logger_->info("level: {}", it.level_);
                  logger_->info("job: {}", it.job_);
                  logger_->info("race: {}", it.race_);
                  logger_->info("face: {}", it.face_);
                  logger_->info("hair: {}", it.hair_);
                  logger_->info("platinium: {}", it.platinum_);
                  logger_->info("seconds until delete: {}", it.remain_sec_unitl_delete_);
                  logger_->info("items:");
                  for (size_t i = 0; i < SrvCharacterListReply::MAX_EQUIPPED_ITEMS; ++i) {
                    logger_->info("Item in position {}", i);
                    const auto &item = it.items_[i];
                    logger_->info("id: {}", item.id_);
                    if (item.id_ == 0)
                      continue;
                    logger_->info("gemOpt: {}", item.gem_op_);
                    logger_->info("socket: {}", item.socket_);
                    logger_->info("grade: {}", item.grade_);
                  }
                }
                auto packet = CliSelectCharReq(0, 0, 0, reply.getCharacters()[0].name_);
                send(packet);
              }
              break;
            case ePacketType::PAKCC_SWITCH_SERVER:
              logger_->info("Got character select reply");
              {
                auto reply = SrvSwitchServer(buffer);
                ip_ = reply.ip();
                port_ = reply.port();
                sessionId_ = reply.sessionId();
                sessionSeed_ = reply.sessionSeed();
              }
              return false;
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

  virtual bool HandlePacket(uint8_t *buffer) override {
    logger_->trace("MapClient::HandlePacket");
    (void)buffer;
    (void)sessionId_;
    (void)sessionSeed_;
    return false;
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
    log->info("Connecting to login server");
    LoginClient loginClient{std::move(socket)};
    loginClient.connect();
    loginClient.start_recv();
    while (loginClient.is_active())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (loginClient.ip().size()) {
        log->info("Connecting to char server");
        socket = std::make_unique<Core::CNetwork_Asio>();
        socket->init(loginClient.ip(), loginClient.port());
        CharClient charClient{loginClient.sessionId(), std::move(socket)};
        charClient.connect();
        charClient.start_recv();
        while (charClient.is_active())
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (charClient.ip().size()) {
          log->info("Connecting to map server");
          socket = std::make_unique<Core::CNetwork_Asio>();
          socket->init(charClient.ip(), charClient.port());
          MapClient mapClient{charClient.sessionId(), charClient.sessionSeed(), std::move(socket)};
          mapClient.connect();
          mapClient.start_recv();
          while (mapClient.is_active())
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
