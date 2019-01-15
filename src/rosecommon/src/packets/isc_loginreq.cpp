#include "isc_loginreq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> IscLoginReq::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::ISC_SERVER_AUTH, &createPacket<IscLoginReq>);

IscLoginReq::IscLoginReq() : CRosePacket(ePacketType::ISC_SERVER_AUTH) {}

IscLoginReq::IscLoginReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::ISC_SERVER_AUTH, "Not the right packet: " << to_underlying(type()));
  char pass[32];
  *this >> pass;
  password_ = std::string(pass, 32);
	*this >> username_;
}

IscLoginReq::IscLoginReq(const std::string &password, const std::string &username) : CRosePacket(ePacketType::ISC_SERVER_AUTH), password_(password), username_(username) {}

std::string &IscLoginReq::password() {
	return password_;
}

const std::string &IscLoginReq::password() const {
	return password_;
}

std::string &IscLoginReq::username() {
	return username_;
}

const std::string &IscLoginReq::username() const {
	return username_;
}


void IscLoginReq::pack() {
  *this << password_.c_str();
	*this << username_;
}

}
