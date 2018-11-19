#include "isc_serverregister.h"
#include "throwassert.h"

namespace RoseCommon {

IscServerRegister::IscServerRegister() : CRosePacket(ePacketType::ISC_SERVER_REGISTER) {}

IscServerRegister::IscServerRegister(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::ISC_SERVER_REGISTER, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); serverType_ = static_cast<Isc::ServerType>(_tmp); }
	reader.get_string(name_);
	reader.get_string(addr_);
	reader.get_int32_t(port_);
	reader.get_int32_t(right_);
	reader.get_int32_t(id_);
}

IscServerRegister::IscServerRegister(Isc::ServerType serverType, const std::string& name, const std::string& addr, int32_t port, int32_t right, int32_t id) : CRosePacket(ePacketType::ISC_SERVER_REGISTER), serverType_(serverType), name_(name), addr_(addr), port_(port), right_(right), id_(id) {
}

Isc::ServerType IscServerRegister::serverType() const { return serverType_; }

IscServerRegister& IscServerRegister::set_serverType(Isc::ServerType data) { serverType_ = data; return *this; }

const std::string& IscServerRegister::name() const { return name_; }

IscServerRegister& IscServerRegister::set_name(const std::string& data) { name_ = data; return *this; }

const std::string& IscServerRegister::addr() const { return addr_; }

IscServerRegister& IscServerRegister::set_addr(const std::string& data) { addr_ = data; return *this; }

int32_t IscServerRegister::port() const { return port_; }

IscServerRegister& IscServerRegister::set_port(int32_t data) { port_ = data; return *this; }

int32_t IscServerRegister::right() const { return right_; }

IscServerRegister& IscServerRegister::set_right(int32_t data) { right_ = data; return *this; }

int32_t IscServerRegister::id() const { return id_; }

IscServerRegister& IscServerRegister::set_id(int32_t data) { id_ = data; return *this; }


void IscServerRegister::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(serverType_);
	writer.set_string(name_);
	writer.set_string(addr_);
	writer.set_int32_t(port_);
	writer.set_int32_t(right_);
	writer.set_int32_t(id_);
}

IscServerRegister IscServerRegister::create(Isc::ServerType serverType, const std::string& name, const std::string& addr, int32_t port, int32_t right, int32_t id) {


	return IscServerRegister(serverType, name, addr, port, right, id);
}

IscServerRegister IscServerRegister::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return IscServerRegister(reader);
}
std::unique_ptr<IscServerRegister> IscServerRegister::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<IscServerRegister>(reader);
}

}