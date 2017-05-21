#include "srv_loginreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvLoginReply::SrvLoginReply() : CRosePacket(ePacketType::PAKLC_LOGIN_REPLY) {}

SrvLoginReply::SrvLoginReply(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    throw_assert(CRosePacket::type() == ePacketType::PAKLC_LOGIN_REPLY, "Not the right packet: " << to_underlying(CRosePacket::type()));
    *this >> result_;
    *this >> right_;
    *this >> type_;
    if (result_ != 0)
        return;
    char pad;
    *this >> pad;
    while (pad) {
        SrvLoginReply::ServerInfo info;
        *this >> info.name_;
        *this >> info.id_;
        info.test_ = pad == '@' ? true : false;
        servers_.push_back(info);
        *this >> pad;
    }
}

SrvLoginReply::SrvLoginReply(uint8_t result, uint16_t right, uint16_t type) : CRosePacket(ePacketType::PAKLC_LOGIN_REPLY), result_(result), right_(right), type_(type) {}

SrvLoginReply::SrvLoginReply(uint8_t result, uint16_t right, uint16_t type, const std::vector<SrvLoginReply::ServerInfo> &servers) : CRosePacket(ePacketType::PAKLC_LOGIN_REPLY), result_(result), right_(right), type_(type), servers_(servers) {}

uint8_t SrvLoginReply::result() const {
	return result_;
}

uint16_t SrvLoginReply::right() const {
	return right_;
}

uint16_t SrvLoginReply::type() const {
	return type_;
}

std::vector<SrvLoginReply::ServerInfo> &SrvLoginReply::servers() {
	return servers_;
}

const std::vector<SrvLoginReply::ServerInfo> &SrvLoginReply::servers() const {
	return servers_;
}

void SrvLoginReply::addServer(const std::string &name, uint32_t id, bool isTest) {
    servers_.push_back(SrvLoginReply::ServerInfo{name, id, isTest});
}


void SrvLoginReply::pack() {
	*this << result_;
	*this << right_;
	*this << type_;
    for (auto &server : servers_) {
        char pad = ' ';
        if (server.test_)
            pad = '@';
        *this << pad << server.name_ << server.id_;
    }
}

}
