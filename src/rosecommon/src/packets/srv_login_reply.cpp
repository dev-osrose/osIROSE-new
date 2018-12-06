#include "srv_login_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;


void SrvLoginReply::ServerInfo::set_test(const char test) {
    this->test = test;
}

char SrvLoginReply::ServerInfo::get_test() const {
    return test;
}

void SrvLoginReply::ServerInfo::set_name(const std::string name) {
    this->name = name;
}

std::string SrvLoginReply::ServerInfo::get_name() const {
    return name;
}

void SrvLoginReply::ServerInfo::set_id(const uint32_t id) {
    this->id = id;
}

uint32_t SrvLoginReply::ServerInfo::get_id() const {
    return id;
}

bool SrvLoginReply::ServerInfo::write(CRoseBasePolicy& writer) const {
    if (!writer.set_char(test)) {
        return false;
    }
    if (!writer.set_string(name)) {
        return false;
    }
    if (!writer.set_uint32_t(id)) {
        return false;
    }
    return true;
}

bool SrvLoginReply::ServerInfo::read(CRoseReader& reader) {
    if (!reader.get_char(test)) {
        return false;
    }
    if (!reader.get_string(name)) {
        return false;
    }
    if (!reader.get_uint32_t(id)) {
        return false;
    }
    return true;
}

constexpr size_t SrvLoginReply::ServerInfo::size() {
    size_t size = 0;
    size += sizeof(char);
    size += sizeof(uint32_t);
    return size;
}


SrvLoginReply::SrvLoginReply() : CRosePacket(ePacketType::PAKLC_LOGIN_REPLY) {}

SrvLoginReply::SrvLoginReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)result)) {
        return;
    }
    if (!reader.get_uint16_t(right)) {
        return;
    }
    if (!reader.get_uint16_t(type)) {
        return;
    }
    {
        SrvLoginReply::ServerInfo elem;
        while (reader.get_iserialize(elem)) {
            serversInfo.push_back(elem);
        }
    }
}

void SrvLoginReply::set_result(const SrvLoginReply::Result result) {
    this->result = result;
}

SrvLoginReply::Result SrvLoginReply::get_result() const {
    return result;
}

void SrvLoginReply::set_right(const uint16_t right) {
    this->right = right;
}

uint16_t SrvLoginReply::get_right() const {
    return right;
}

void SrvLoginReply::set_type(const uint16_t type) {
    this->type = type;
}

uint16_t SrvLoginReply::get_type() const {
    return type;
}

void SrvLoginReply::set_serversInfo(const std::vector<SrvLoginReply::ServerInfo> serversInfo) {
    this->serversInfo = serversInfo;
}

void SrvLoginReply::add_serversInfo(const ServerInfo serversInfo) {
    this->serversInfo.emplace_back(serversInfo);
}

std::vector<SrvLoginReply::ServerInfo> SrvLoginReply::get_serversInfo() const {
    return serversInfo;
}

SrvLoginReply::ServerInfo SrvLoginReply::get_serversInfo(size_t index) const {
    return serversInfo[index];
}

SrvLoginReply SrvLoginReply::create(const SrvLoginReply::Result& result, const uint16_t& right, const uint16_t& type) {
    SrvLoginReply packet;
    packet.set_result(result);
    packet.set_right(right);
    packet.set_type(type);
    return packet;
}

SrvLoginReply SrvLoginReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvLoginReply(reader);
}

void SrvLoginReply::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(result)) {
        return;
    }
    if (!writer.set_uint16_t(right)) {
        return;
    }
    if (!writer.set_uint16_t(type)) {
        return;
    }
    for (const auto& elem : serversInfo) {
        if (!writer.set_iserialize(elem)) {
            return;
        }
    }
}

constexpr size_t SrvLoginReply::size() {
    size_t size = 0;
    size += sizeof(Result);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += ServerInfo::size();
    return size;
}

