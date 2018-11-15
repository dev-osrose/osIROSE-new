#include "srv_switchserver.h"
#include "throwassert.h"

namespace RoseCommon {

SrvSwitchServer::SrvSwitchServer() : CRosePacket(ePacketType::PAKCC_SWITCH_SERVER) {}

SrvSwitchServer::SrvSwitchServer(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCC_SWITCH_SERVER, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(port_);
	reader.get_uint32_t(sessionId_);
	reader.get_uint32_t(sessionSeed_);
	reader.get_string(ip_);
}

SrvSwitchServer::SrvSwitchServer(uint16_t port, uint32_t sessionId, uint32_t sessionSeed, const std::string& ip) : CRosePacket(ePacketType::PAKCC_SWITCH_SERVER), port_(port), sessionId_(sessionId), sessionSeed_(sessionSeed), ip_(ip) {
}

uint16_t SrvSwitchServer::port() const { return port_; }

SrvSwitchServer& SrvSwitchServer::set_port(uint16_t data) { port_ = data; return *this; }

uint32_t SrvSwitchServer::sessionId() const { return sessionId_; }

SrvSwitchServer& SrvSwitchServer::set_sessionId(uint32_t data) { sessionId_ = data; return *this; }

uint32_t SrvSwitchServer::sessionSeed() const { return sessionSeed_; }

SrvSwitchServer& SrvSwitchServer::set_sessionSeed(uint32_t data) { sessionSeed_ = data; return *this; }

const std::string& SrvSwitchServer::ip() const { return ip_; }

SrvSwitchServer& SrvSwitchServer::set_ip(const std::string& data) { ip_ = data; return *this; }


void SrvSwitchServer::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(port_);
	writer.set_uint32_t(sessionId_);
	writer.set_uint32_t(sessionSeed_);
	writer.set_string(ip_);
}

uint16_t SrvSwitchServer::get_size() const {
	uint16_t size = 0;
	size += sizeof(port_);
	size += sizeof(sessionId_);
	size += sizeof(sessionSeed_);
	size += sizeof(char) * (ip_.size() + 1);
	return size;
}


SrvSwitchServer SrvSwitchServer::create(uint16_t port, uint32_t sessionId, uint32_t sessionSeed, const std::string& ip) {


	return SrvSwitchServer(port, sessionId, sessionSeed, ip);
}

SrvSwitchServer SrvSwitchServer::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvSwitchServer(reader);
}

}