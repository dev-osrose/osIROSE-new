#include "map_client.h"

#include "cli_join_server_req.h"
#include "cli_change_map_req.h"
#include "cli_normal_chat.h"

#include <stdexcept>

class OkException : std::exception {};

using namespace RoseCommon;
using namespace RoseCommon::Packet;

MapClient::MapClient(uint32_t sessionId, uint32_t sessionSeed,
                     std::unique_ptr<Core::INetwork> sock) :
    Client(std::move(sock)), sessionId_(sessionId), sessionSeed_(sessionSeed) {
    register_dispatcher<SrvAcceptReply>(std::bind(&MapClient::accept_reply, this, std::placeholders::_1));
    register_dispatcher<SrvSelectCharReply>(std::bind(&MapClient::select_char_reply, this, std::placeholders::_1));
    register_dispatcher<SrvInventoryData>(std::bind(&MapClient::inventory_data, this, std::placeholders::_1));
    register_dispatcher<SrvQuestData>(std::bind(&MapClient::quest_data, this, std::placeholders::_1));
    register_dispatcher<SrvBillingMessage>(std::bind(&MapClient::billing_message, this, std::placeholders::_1));
    register_dispatcher<SrvChangeMapReply>(std::bind(&MapClient::change_map_reply, this, std::placeholders::_1));
    register_dispatcher<SrvWhisperChat>(std::bind(&MapClient::whisper_chat, this, std::placeholders::_1));
}

bool MapClient::handlePacket(uint8_t *buffer) {
    logger_->trace("MapClient::HandlePacket");
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

void MapClient::accept_reply(const SrvAcceptReply&) {
    logger_->info("Got accept reply");
    logger_->info("asking for joinning server");
    auto packet = CliJoinServerReq::create(
        sessionId_,
        std::string("098f6bcd4621d373cade4e832627b4f6"));
    send(packet);
}

void MapClient::join_server_reply(const SrvJoinServerReply&) {
    logger_->info("Logged in.");
}

void MapClient::select_char_reply(const SrvSelectCharReply&) {
    logger_->info("Got char data reply");
}

void MapClient::inventory_data(const SrvInventoryData&) {
    logger_->info("Got inventory data reply");
}

void MapClient::quest_data(const SrvQuestData&) {
    logger_->info("Got quest data reply");
}

void MapClient::billing_message(const SrvBillingMessage&) {
    logger_->info("Got billing message reply");
    logger_->info("Sending change map request");
    auto packet = CliChangeMapReq::create(0, 0);
    send(packet);
}

void MapClient::change_map_reply(const SrvChangeMapReply&) {
    logger_->info("Got change map reply, we are now on the map");
    logger_->info("sending broadcast message");
    auto packet = CliNormalChat::create("/whisper botName this is a test message");
    send(packet);
}

void MapClient::whisper_chat(const SrvWhisperChat& packet) {
    logger_->info("Got whisper chat");
    logger_->info("From {}, message {}", packet.get_sender(), packet.get_message());
}