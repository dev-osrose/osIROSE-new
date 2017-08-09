#include "isc_serverregister.h"
#include "throwassert.h"

namespace RoseCommon {

IscServerRegister::IscServerRegister() : CRosePacket(ePacketType::ISC_SERVER_REGISTER) {}

IscServerRegister::IscServerRegister(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::ISC_SERVER_REGISTER, "Not the right packet: " << to_underlying(type()));
	*this >> type_;
	*this >> name_;
	*this >> addr_;
	*this >> port_;
	*this >> right_;
	*this >> id_;
}

  IscServerRegister::IscServerRegister(const std::string &name, const std::string &ip, int32_t id, int32_t port, Isc::ServerType type, int32_t right) : CRosePacket(ePacketType::ISC_SERVER_REGISTER), type_(type), name_(name), addr_(ip), port_(port), right_(right), id_(id) {}

Isc::ServerType IscServerRegister::serverType() const {
	return type_;
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
	*this << to_underlying(type_);
	*this << name_;
	*this << addr_;
	*this << port_;
	*this << right_;
	*this << id_;
}

}
