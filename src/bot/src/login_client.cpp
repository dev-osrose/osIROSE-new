#include "login_client.h"

#include <stdexcept>

#include "cli_login_req.h"
#include "cli_channel_list_req.h"
#include "cli_srv_select_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

class OkException : std::exception {};

LoginClient::LoginClient(std::unique_ptr<Core::INetwork> sock) : Client(std::move(sock)) {
    register_dispatcher<SrvAcceptReply>(std::bind(&LoginClient::accept_reply, this, std::placeholders::_1));
    register_dispatcher<SrvLoginReply>(std::bind(&LoginClient::login_reply, this, std::placeholders::_1));
    register_dispatcher<SrvChannelListReply>(std::bind(&LoginClient::channel_list_reply, this, std::placeholders::_1));
    register_dispatcher<SrvSrvSelectReply>(std::bind(&LoginClient::srv_select_reply, this, std::placeholders::_1));
}

uint32_t LoginClient::sessionId() const {
    return sessionId_;
}

std::string LoginClient::ip() const {
    return ip_;
}

uint16_t LoginClient::port() const {
    return port_;
}

bool LoginClient::handlePacket(uint8_t *buffer) {
    logger_->trace("LoginClient::HandlePacket");
    auto packet = RoseCommon::fetchPacket<true>(buffer);
    if (!dispatcher.is_supported(*packet.get())) {
        logger_->info("Received a packet : 0x{0:04x}", (uint16_t)CRosePacket::type(buffer));
        return false;
    } else {
        try {
            dispatcher.dispatch(std::move(packet));
        } catch (const OkException&) {
            return false;
        } catch (const std::runtime_error& e) {
            logger_->error("Error: {}", e.what());
            return false;
        }
    }
    return true;
}

void LoginClient::accept_reply(const SrvAcceptReply&) {
    logger_->info("Got accept reply");
    logger_->info("Trying to login");
    auto packet = CliLoginReq::create(
        std::string("098f6bcd4621d373cade4e832627b4f6"),
        "test2");
    send(packet);
}

void LoginClient::login_reply(const SrvLoginReply& reply) {
    logger_->info("Got a login reply");
    logger_->info("Got reply result: {}, right: {}, type: {}", reply.get_result(), reply.get_right(), reply.get_type());
    if (reply.get_result() != 0) {
        throw std::runtime_error("Error wrong result");
    }
    logger_->info("Server list:");
    for (const auto &server : reply.get_serversInfo())
        logger_->info("{}({}) : test = {}", server.get_name(), server.get_id(), server.get_test());
    if (reply.get_serversInfo().empty()) {
        throw std::runtime_error("No server available");
    }
    auto packet = CliChannelListReq::create(reply.get_serversInfo()[0].get_id());
    send(packet);
}

void LoginClient::channel_list_reply(const SrvChannelListReply& reply) {
    logger_->info("Got channel list reply");
    logger_->info("Got channels for server: {}", reply.get_id());
    if (!reply.get_channels().size()) {
        throw std::runtime_error("No available channel");
    }
    logger_->info("Channel list:");
    for (const auto &channel : reply.get_channels())
        logger_->info("{}({}): {}-{}, {}%", channel.get_name(),
                channel.get_id(), channel.get_lowAge(), channel.get_highAge(), channel.get_capacity());
    auto packet = CliSrvSelectReq::create(reply.get_id(), reply.get_channels()[0].get_id());
    send(packet);
}

void LoginClient::srv_select_reply(const SrvSrvSelectReply& reply) {
    logger_->info("Got server select reply");
    logger_->info("Got reply result: {}", reply.get_result());
    if (reply.get_result() != SrvSrvSelectReply::OK) {
        throw std::runtime_error("Error wrong result");
    }
    sessionId_ = reply.get_sessionId();
    ip_ = reply.get_ip();
    port_ = reply.get_port();
    throw OkException{};
}