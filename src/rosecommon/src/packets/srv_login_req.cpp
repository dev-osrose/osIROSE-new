#include "srv_login_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;


void SrvLoginReq::ChannelInfo::set_id(const uint8_t id) {
    this->id = id;
}

uint8_t SrvLoginReq::ChannelInfo::get_id() const {
    return id;
}

void SrvLoginReq::ChannelInfo::set_lowAge(const uint8_t lowAge) {
    this->lowAge = lowAge;
}

uint8_t SrvLoginReq::ChannelInfo::get_lowAge() const {
    return lowAge;
}

void SrvLoginReq::ChannelInfo::set_highAge(const uint8_t highAge) {
    this->highAge = highAge;
}

uint8_t SrvLoginReq::ChannelInfo::get_highAge() const {
    return highAge;
}

void SrvLoginReq::ChannelInfo::set_capacity(const uint16_t capacity) {
    this->capacity = capacity;
}

uint16_t SrvLoginReq::ChannelInfo::get_capacity() const {
    return capacity;
}

void SrvLoginReq::ChannelInfo::set_name(const std::string& name) {
    this->name = name;
}

const std::string& SrvLoginReq::ChannelInfo::get_name() const {
    return name;
}

bool SrvLoginReq::ChannelInfo::write(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(id)) {
        return false;
    }
    if (!writer.set_uint8_t(lowAge)) {
        return false;
    }
    if (!writer.set_uint8_t(highAge)) {
        return false;
    }
    if (!writer.set_uint16_t(capacity)) {
        return false;
    }
    if (!writer.set_string(name)) {
        return false;
    }
    return true;
}

bool SrvLoginReq::ChannelInfo::read(CRoseReader& reader) {
    if (!reader.get_uint8_t(id)) {
        return false;
    }
    if (!reader.get_uint8_t(lowAge)) {
        return false;
    }
    if (!reader.get_uint8_t(highAge)) {
        return false;
    }
    if (!reader.get_uint16_t(capacity)) {
        return false;
    }
    if (!reader.get_string(name)) {
        return false;
    }
    return true;
}

constexpr size_t SrvLoginReq::ChannelInfo::size() {
    size_t size = 0;
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    size += sizeof(uint16_t);
    return size;
}


SrvLoginReq::SrvLoginReq() : CRosePacket(ePacketType::PAKLC_LOGIN_REQ) {}

SrvLoginReq::SrvLoginReq(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint32_t(id)) {
        return;
    }
    {
        uint8_t size;
        if (!reader.get_uint8_t(size)) {
            return;
        }
        while (size-- > 0) {
            SrvLoginReq::ChannelInfo elem;
            if (!reader.get_iserialize(elem)) {
                return;
            }
            channels.push_back(elem);
        }
    }
}

void SrvLoginReq::set_id(const uint32_t id) {
    this->id = id;
}

uint32_t SrvLoginReq::get_id() const {
    return id;
}

void SrvLoginReq::set_channels(const std::vector<SrvLoginReq::ChannelInfo>& channels) {
    this->channels = channels;
}

void SrvLoginReq::add_channels(const ChannelInfo& channels) {
    this->channels.emplace_back(channels);
}

const std::vector<SrvLoginReq::ChannelInfo>& SrvLoginReq::get_channels() const {
    return channels;
}

const SrvLoginReq::ChannelInfo& SrvLoginReq::get_channels(size_t index) const {
    return channels[index];
}

SrvLoginReq SrvLoginReq::create(const uint32_t& id) {
    SrvLoginReq packet;
    packet.set_id(id);
    return packet;
}

SrvLoginReq SrvLoginReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvLoginReq(reader);
}

void SrvLoginReq::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint32_t(id)) {
        return;
    }
    if (!writer.set_uint8_t(channels.size())) {
        return;
    }
    for (const auto& elem : channels) {
        if (!writer.set_iserialize(elem)) {
            return;
        }
    }
}

constexpr size_t SrvLoginReq::size() {
    size_t size = 0;
    size += sizeof(uint32_t);
    size += sizeof(uint8_t);
    size += ChannelInfo::size();
    return size;
}

