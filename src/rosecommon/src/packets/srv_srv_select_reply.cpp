#include "srv_srv_select_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvSrvSelectReply::SrvSrvSelectReply() : CRosePacket(ePacketType::PAKLC_SRV_SELECT_REPLY) {}

SrvSrvSelectReply::SrvSrvSelectReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)result)) {
        return;
    }
    if (!reader.get_uint32_t(sessionId)) {
        return;
    }
    if (!reader.get_uint32_t(cryptVal)) {
        return;
    }
    if (!reader.get_string(ip)) {
        return;
    }
    if (!reader.get_uint16_t(port)) {
        return;
    }
}

void SrvSrvSelectReply::set_result(const SrvSrvSelectReply::Result result) {
    this->result = result;
}

SrvSrvSelectReply::Result SrvSrvSelectReply::get_result() const {
    return result;
}

void SrvSrvSelectReply::set_sessionId(const uint32_t sessionId) {
    this->sessionId = sessionId;
}

uint32_t SrvSrvSelectReply::get_sessionId() const {
    return sessionId;
}

void SrvSrvSelectReply::set_cryptVal(const uint32_t cryptVal) {
    this->cryptVal = cryptVal;
}

uint32_t SrvSrvSelectReply::get_cryptVal() const {
    return cryptVal;
}

void SrvSrvSelectReply::set_ip(const std::string& ip) {
    this->ip = ip;
}

const std::string& SrvSrvSelectReply::get_ip() const {
    return ip;
}

void SrvSrvSelectReply::set_port(const uint16_t port) {
    this->port = port;
}

uint16_t SrvSrvSelectReply::get_port() const {
    return port;
}

SrvSrvSelectReply SrvSrvSelectReply::create(const SrvSrvSelectReply::Result& result, const uint32_t& sessionId, const uint32_t& cryptVal, const std::string& ip, const uint16_t& port) {
    SrvSrvSelectReply packet;
    packet.set_result(result);
    packet.set_sessionId(sessionId);
    packet.set_cryptVal(cryptVal);
    packet.set_ip(ip);
    packet.set_port(port);
    return packet;
}

SrvSrvSelectReply SrvSrvSelectReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvSrvSelectReply(reader);
}

void SrvSrvSelectReply::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(result)) {
        return;
    }
    if (!writer.set_uint32_t(sessionId)) {
        return;
    }
    if (!writer.set_uint32_t(cryptVal)) {
        return;
    }
    if (!writer.set_string(ip)) {
        return;
    }
    if (!writer.set_uint16_t(port)) {
        return;
    }
}

constexpr size_t SrvSrvSelectReply::size() {
    size_t size = 0;
    size += sizeof(Result);
    size += sizeof(uint32_t);
    size += sizeof(uint32_t);
    size += sizeof(uint16_t);
    return size;
}
