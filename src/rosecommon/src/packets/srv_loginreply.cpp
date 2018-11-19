#include "srv_loginreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvLoginReply::SrvLoginReply() : CRosePacket(ePacketType::PAKLC_LOGIN_REPLY) {}

SrvLoginReply::SrvLoginReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKLC_LOGIN_REPLY, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); result_ = static_cast<LoginReply::eResult>(_tmp); }
	reader.get_uint16_t(right_);
	reader.get_uint16_t(type_);
	{
		LoginReply::ServerInfo tmp;
		while (reader.get_iserialize(tmp)) {
			reader.get_iserialize(tmp);
			serversInfo_.push_back(tmp);
		}
	}
}

SrvLoginReply::SrvLoginReply(LoginReply::eResult result, uint16_t right, uint16_t type) : CRosePacket(ePacketType::PAKLC_LOGIN_REPLY), result_(result), right_(right), type_(type) {
}

LoginReply::eResult SrvLoginReply::result() const { return result_; }

SrvLoginReply& SrvLoginReply::set_result(LoginReply::eResult data) { result_ = data; return *this; }

uint16_t SrvLoginReply::right() const { return right_; }

SrvLoginReply& SrvLoginReply::set_right(uint16_t data) { right_ = data; return *this; }

uint16_t SrvLoginReply::type() const { return type_; }

SrvLoginReply& SrvLoginReply::set_type(uint16_t data) { type_ = data; return *this; }

const std::vector<LoginReply::ServerInfo>& SrvLoginReply::serversInfo() const { return serversInfo_; }

SrvLoginReply& SrvLoginReply::set_serversInfo(const std::vector<LoginReply::ServerInfo>& data) { serversInfo_ = data; return *this; }

SrvLoginReply& SrvLoginReply::add_serverinfo(const LoginReply::ServerInfo& data) { serversInfo_.push_back(data); return *this; }


void SrvLoginReply::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(result_);
	writer.set_uint16_t(right_);
	writer.set_uint16_t(type_);
	for (const auto& elem : serversInfo_) writer.set_iserialize(elem);
}

SrvLoginReply SrvLoginReply::create(LoginReply::eResult result, uint16_t right, uint16_t type) {


	return SrvLoginReply(result, right, type);
}

SrvLoginReply SrvLoginReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvLoginReply(reader);
}
std::unique_ptr<SrvLoginReply> SrvLoginReply::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvLoginReply>(reader);
}

}