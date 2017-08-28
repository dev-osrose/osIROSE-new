#include "isc_serverregister.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> IscServerRegister::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::ISC_SERVER_REGISTER, &createPacket<IscServerRegister>);

IscServerRegister::IscServerRegister() : CRosePacket(ePacketType::ISC_SERVER_REGISTER) {}

IscServerRegister::IscServerRegister(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::ISC_SERVER_REGISTER, "Not the right packet: " << to_underlying(type()));
	*this >> serverType_;
	*this >> name_;
	*this >> addr_;
	*this >> port_;
	*this >> right_;
	*this >> id_;
}

IscServerRegister::IscServerRegister(Isc::ServerType serverType, const std::string &name, const std::string &addr, int32_t port, int32_t right, int32_t id) : CRosePacket(ePacketType::ISC_SERVER_REGISTER), serverType_(serverType), name_(name), addr_(addr), port_(port), right_(right), id_(id) {}

Isc::ServerType IscServerRegister::serverType() const {
	return serverType_;
}

std::string &IscServerRegister::name() {
	return name_;
}

const std::string &IscServerRegister::name() const {
	return name_;
}

std::string &IscServerRegister::addr() {
	return addr_;
}

const std::string &IscServerRegister::addr() const {
	return addr_;
}

int32_t IscServerRegister::port() const {
	return port_;
}

int32_t IscServerRegister::right() const {
	return right_;
}

int32_t IscServerRegister::id() const {
	return id_;
}


void IscServerRegister::pack() {
	*this << serverType_;
	*this << name_;
	*this << addr_;
	*this << port_;
	*this << right_;
	*this << id_;
}

}
