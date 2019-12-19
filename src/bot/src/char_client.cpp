#include "char_client.h"
#include "enumerate.h"

#include "cli_join_server_req.h"
#include "cli_create_char_req.h"
#include "cli_char_list_req.h"
#include "cli_select_char_req.h"

#include <stdexcept>

using namespace RoseCommon;
using namespace RoseCommon::Packet;

class OkException : std::exception {};

CharClient::CharClient(uint32_t sessionId, std::unique_ptr<Core::INetwork> sock):
    Client(std::move(sock)), sessionId_(sessionId) {
    register_dispatcher<SrvAcceptReply>(std::bind(&CharClient::accept_reply, this, std::placeholders::_1));
    register_dispatcher<SrvJoinServerReply>(std::bind(&CharClient::join_server_reply, this, std::placeholders::_1));
    register_dispatcher<SrvCharListReply>(std::bind(&CharClient::char_list_reply, this, std::placeholders::_1));
    register_dispatcher<SrvCreateCharReply>(std::bind(&CharClient::create_char_reply, this, std::placeholders::_1));
    register_dispatcher<SrvSwitchServer>(std::bind(&CharClient::switch_server, this, std::placeholders::_1));
}

uint32_t CharClient::sessionId() const {
    return sessionId_;
}

uint16_t CharClient::port() const {
    return port_;
}

uint32_t CharClient::sessionSeed() const {
    return sessionSeed_;
}

const std::string &CharClient::ip() const {
    return ip_;
}

bool CharClient::handlePacket(uint8_t *buffer) {
    logger_->trace("CharClient::HandlePacket");
    auto packet = RoseCommon::fetchPacket<true>(buffer);
    if (!dispatcher.is_supported(*packet.get())) {
        logger_->error("Received a packet : 0x{0:04x}", (uint16_t)CRosePacket::type(buffer));
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

void CharClient::accept_reply(const SrvAcceptReply&) {
    logger_->info("Got accept reply");
    logger_->info("asking for joinning server");
    auto packet = CliJoinServerReq::create(
        sessionId_,
        std::string("098f6bcd4621d373cade4e832627b4f6"));
    send(packet);
}

void CharClient::join_server_reply(const SrvJoinServerReply& reply) {
    logger_->info("Got join server reply:");
    logger_->info("Reply: {}, id: {}, payFlag: {}", (uint8_t)reply.get_result(), reply.get_id(), reply.get_payFlag());
    if (reply.get_result() != SrvJoinServerReply::OK) {
        throw std::runtime_error("Wrong result");
    }
    auto packet = CliCharListReq();
    send(packet);
}

void CharClient::char_list_reply(const SrvCharListReply& reply) {
    logger_->info("Got character list");
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
        for (const auto& [i, item] : Core::enumerate(it.get_items())) {
            logger_->info("Item in position {}", i);
            logger_->info("id: {}", item.get_id());
            if (item.get_id() == 0) {
                continue;
            }
            logger_->info("gemOpt: {}", item.get_gem_opt());
            logger_->info("socket: {}", item.get_socket());
            logger_->info("grade: {}", item.get_grade());
        }
    }
    if (reply.get_characters().size() == 0) {
        logger_->info("No character! Creating one");
        auto packet = CliCreateCharReq::create(0, 0, 0, 0, 0, 0, fmt::format("botName{}", sessionId_ % 100));
        send(packet);
    } else {
        auto packet = CliSelectCharReq::create(0, 0, 0, reply.get_characters()[0].get_name());
        send(packet);
    }
}

void CharClient::create_char_reply(const SrvCreateCharReply&) {
    logger_->info("Character created");
    auto packet = CliCharListReq();
    send(packet);
}

void CharClient::switch_server(const SrvSwitchServer& reply) {
        logger_->info("Got character select reply");
        ip_ = reply.get_ip();
        port_ = reply.get_port();
        sessionId_ = reply.get_sessionId();
        sessionSeed_ = reply.get_sessionSeed();
        throw OkException{};
}