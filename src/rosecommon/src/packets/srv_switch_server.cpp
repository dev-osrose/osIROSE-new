#include "srv_switch_server.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvSwitchServer::SrvSwitchServer() : CRosePacket(SrvSwitchServer::PACKET_ID) {}

SrvSwitchServer::SrvSwitchServer(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(port)) {
        return;
    }
    if (!reader.get_uint32_t(sessionId)) {
        return;
    }
    if (!reader.get_uint32_t(sessionSeed)) {
        return;
    }
    if (!reader.get_string(ip)) {
        return;
    }
}

void SrvSwitchServer::set_port(const uint16_t port) {
    this->port = port;
}

uint16_t SrvSwitchServer::get_port() const {
    return port;
}

void SrvSwitchServer::set_sessionId(const uint32_t sessionId) {
    this->sessionId = sessionId;
}

uint32_t SrvSwitchServer::get_sessionId() const {
    return sessionId;
}

void SrvSwitchServer::set_sessionSeed(const uint32_t sessionSeed) {
    this->sessionSeed = sessionSeed;
}

uint32_t SrvSwitchServer::get_sessionSeed() const {
    return sessionSeed;
}

void SrvSwitchServer::set_ip(const std::string& ip) {
    this->ip = ip;
}

const std::string& SrvSwitchServer::get_ip() const {
    return ip;
}

SrvSwitchServer SrvSwitchServer::create(const uint16_t& port, const uint32_t& sessionId, const uint32_t& sessionSeed, const std::string& ip) {
    SrvSwitchServer packet;
    packet.set_port(port);
    packet.set_sessionId(sessionId);
    packet.set_sessionSeed(sessionSeed);
    packet.set_ip(ip);
    return packet;
}

SrvSwitchServer SrvSwitchServer::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvSwitchServer(reader);
}

std::unique_ptr<SrvSwitchServer> SrvSwitchServer::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvSwitchServer>(reader);
}

void SrvSwitchServer::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(port)) {
        return;
    }
    if (!writer.set_uint32_t(sessionId)) {
        return;
    }
    if (!writer.set_uint32_t(sessionSeed)) {
        return;
    }
    if (!writer.set_string(ip)) {
        return;
    }
}

constexpr size_t SrvSwitchServer::size() {
    size_t size = 0;
    size += sizeof(uint16_t); // port
    size += sizeof(uint32_t); // sessionId
    size += sizeof(uint32_t); // sessionSeed
    return size;
}

