#include "cli_loginreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliLoginReq::CliLoginReq() : CRosePacket(ePacketType::PAKCS_LOGIN_REQ) {}

CliLoginReq::CliLoginReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_LOGIN_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_string(password_, 32);
	reader.get_string(username_);
}

CliLoginReq::CliLoginReq(const std::string& password, const std::string& username) : CRosePacket(ePacketType::PAKCS_LOGIN_REQ), password_(password), username_(username) {
}

const std::string& CliLoginReq::password() const { return password_; }

CliLoginReq& CliLoginReq::set_password(const std::string& data) { password_ = data; return *this; }

const std::string& CliLoginReq::username() const { return username_; }

CliLoginReq& CliLoginReq::set_username(const std::string& data) { username_ = data; return *this; }


void CliLoginReq::pack(CRoseBasePolicy& writer) const {
	writer.set_string(password_, 32);
	writer.set_string(username_);
}

CliLoginReq CliLoginReq::create(const std::string& password, const std::string& username) {


	return CliLoginReq(password, username);
}

CliLoginReq CliLoginReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliLoginReq(reader);
}
std::unique_ptr<CliLoginReq> CliLoginReq::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliLoginReq>(reader);
}

}