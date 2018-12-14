#include "srv_channel_list_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;


void SrvChannelListReply::ChannelInfo::set_id(const uint8_t id) {
    this->id = id;
}

uint8_t SrvChannelListReply::ChannelInfo::get_id() const {
    return id;
}

void SrvChannelListReply::ChannelInfo::set_lowAge(const uint8_t lowAge) {
    this->lowAge = lowAge;
}

uint8_t SrvChannelListReply::ChannelInfo::get_lowAge() const {
    return lowAge;
}

void SrvChannelListReply::ChannelInfo::set_highAge(const uint8_t highAge) {
    this->highAge = highAge;
}

uint8_t SrvChannelListReply::ChannelInfo::get_highAge() const {
    return highAge;
}

void SrvChannelListReply::ChannelInfo::set_capacity(const uint16_t capacity) {
    this->capacity = capacity;
}

uint16_t SrvChannelListReply::ChannelInfo::get_capacity() const {
    return capacity;
}

void SrvChannelListReply::ChannelInfo::set_name(const std::string name) {
    this->name = name;
}

std::string SrvChannelListReply::ChannelInfo::get_name() const {
    return name;
}

bool SrvChannelListReply::ChannelInfo::write(CRoseBasePolicy& writer) const {
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

bool SrvChannelListReply::ChannelInfo::read(CRoseReader& reader) {
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

constexpr size_t SrvChannelListReply::ChannelInfo::size() {
    size_t size = 0;
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    size += sizeof(uint16_t);
    return size;
}


SrvChannelListReply::SrvChannelListReply() : CRosePacket(SrvChannelListReply::PACKET_ID) {}

SrvChannelListReply::SrvChannelListReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint32_t(id)) {
        return;
    }
    {
        uint8_t size;
        if (!reader.get_uint8_t(size)) {
            return;
        }
        while (size-- > 0) {
            SrvChannelListReply::ChannelInfo elem;
            if (!reader.get_iserialize(elem)) {
                return;
            }
            channels.push_back(elem);
        }
    }
}

void SrvChannelListReply::set_id(const uint32_t id) {
    this->id = id;
}

uint32_t SrvChannelListReply::get_id() const {
    return id;
}

void SrvChannelListReply::set_channels(const std::vector<SrvChannelListReply::ChannelInfo> channels) {
    this->channels = channels;
}

void SrvChannelListReply::add_channels(const ChannelInfo channels) {
    this->channels.emplace_back(channels);
}

std::vector<SrvChannelListReply::ChannelInfo> SrvChannelListReply::get_channels() const {
    return channels;
}

SrvChannelListReply::ChannelInfo SrvChannelListReply::get_channels(size_t index) const {
    return channels[index];
}

SrvChannelListReply SrvChannelListReply::create(const uint32_t& id) {
    SrvChannelListReply packet;
    packet.set_id(id);
    return packet;
}

SrvChannelListReply SrvChannelListReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvChannelListReply(reader);
}

std::unique_ptr<SrvChannelListReply> SrvChannelListReply::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvChannelListReply>(reader);
}

void SrvChannelListReply::pack(CRoseBasePolicy& writer) const {
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

constexpr size_t SrvChannelListReply::size() {
    size_t size = 0;
    size += sizeof(uint32_t);
    size += sizeof(uint8_t);
    size += ChannelInfo::size();
    return size;
}

