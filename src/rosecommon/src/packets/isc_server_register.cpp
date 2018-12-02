#include "isc_server_register.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



IscServerRegister::IscServerRegister() : CRosePacket(ePacketType::ISC_SERVER_REGISTER) {}

IscServerRegister::IscServerRegister(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)serverType)) {
        return;
    }
    if (!reader.get_string(name)) {
        return;
    }
    if (!reader.get_string(addr)) {
        return;
    }
    if (!reader.get_int32_t(port)) {
        return;
    }
    if (!reader.get_int32_t(right)) {
        return;
    }
    if (!reader.get_int32_t(id)) {
        return;
    }
}

void IscServerRegister::set_serverType(const Isc::ServerType serverType) {
    this->serverType = serverType;
}

Isc::ServerType IscServerRegister::get_serverType() const {
    return serverType;
}

void IscServerRegister::set_name(const std::string& name) {
    this->name = name;
}

const std::string& IscServerRegister::get_name() const {
    return name;
}

void IscServerRegister::set_addr(const std::string& addr) {
    this->addr = addr;
}

const std::string& IscServerRegister::get_addr() const {
    return addr;
}

void IscServerRegister::set_port(const int32_t port) {
    this->port = port;
}

int32_t IscServerRegister::get_port() const {
    return port;
}

void IscServerRegister::set_right(const int32_t right) {
    this->right = right;
}

int32_t IscServerRegister::get_right() const {
    return right;
}

void IscServerRegister::set_id(const int32_t id) {
    this->id = id;
}

int32_t IscServerRegister::get_id() const {
    return id;
}

IscServerRegister IscServerRegister::create(const Isc::ServerType& serverType, const std::string& name, const std::string& addr, const int32_t& port, const int32_t& right, const int32_t& id) {
    IscServerRegister packet;
    packet.set_serverType(serverType);
    packet.set_name(name);
    packet.set_addr(addr);
    packet.set_port(port);
    packet.set_right(right);
    packet.set_id(id);
    return packet;
}

IscServerRegister IscServerRegister::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return IscServerRegister(reader);
}

void IscServerRegister::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(serverType)) {
        return;
    }
    if (!writer.set_string(name)) {
        return;
    }
    if (!writer.set_string(addr)) {
        return;
    }
    if (!writer.set_int32_t(port)) {
        return;
    }
    if (!writer.set_int32_t(right)) {
        return;
    }
    if (!writer.set_int32_t(id)) {
        return;
    }
}

constexpr size_t IscServerRegister::size() {
    size_t size = 0;
    size += sizeof(Isc::ServerType);
    size += sizeof(int32_t);
    size += sizeof(int32_t);
    size += sizeof(int32_t);
    return size;
}
