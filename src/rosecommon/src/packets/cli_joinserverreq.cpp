#include "cli_joinserverreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliJoinServerReq::CliJoinServerReq() : CRosePacket(ePacketType::PAKCS_JOIN_SERVER_REQ) {}

CliJoinServerReq::CliJoinServerReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_JOIN_SERVER_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint32_t(sessionId_);
	reader.get_string(password_, 32);
}

CliJoinServerReq::CliJoinServerReq(uint32_t sessionId, const std::string& password) : CRosePacket(ePacketType::PAKCS_JOIN_SERVER_REQ), sessionId_(sessionId), password_(password) {
}

uint32_t CliJoinServerReq::sessionId() const { return sessionId_; }

CliJoinServerReq& CliJoinServerReq::set_sessionId(uint32_t data) { sessionId_ = data; return *this; }

const std::string& CliJoinServerReq::password() const { return password_; }

CliJoinServerReq& CliJoinServerReq::set_password(const std::string& data) { password_ = data; return *this; }


void CliJoinServerReq::pack(CRoseBasePolicy& writer) const {
	writer.set_uint32_t(sessionId_);
	writer.set_string(password_, 32);
}

CliJoinServerReq CliJoinServerReq::create(uint32_t sessionId, const std::string& password) {


	return CliJoinServerReq(sessionId, password);
}

CliJoinServerReq CliJoinServerReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliJoinServerReq(reader);
}
std::unique_ptr<CliJoinServerReq> CliJoinServerReq::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliJoinServerReq>(reader);
}

}