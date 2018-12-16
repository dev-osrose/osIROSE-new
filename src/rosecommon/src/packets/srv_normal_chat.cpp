#include "srv_normal_chat.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvNormalChat::SrvNormalChat() : CRosePacket(SrvNormalChat::PACKET_ID) {}

SrvNormalChat::SrvNormalChat(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(charId)) {
        return;
    }
    if (!reader.get_string(message)) {
        return;
    }
}

void SrvNormalChat::set_charId(const uint16_t charId) {
    this->charId = charId;
}

uint16_t SrvNormalChat::get_charId() const {
    return charId;
}

void SrvNormalChat::set_message(const std::string& message) {
    this->message = message;
}

const std::string& SrvNormalChat::get_message() const {
    return message;
}

SrvNormalChat SrvNormalChat::create(const uint16_t& charId, const std::string& message) {
    SrvNormalChat packet;
    packet.set_charId(charId);
    packet.set_message(message);
    return packet;
}

SrvNormalChat SrvNormalChat::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvNormalChat(reader);
}

std::unique_ptr<SrvNormalChat> SrvNormalChat::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvNormalChat>(reader);
}

void SrvNormalChat::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(charId)) {
        return;
    }
    if (!writer.set_string(message)) {
        return;
    }
}

constexpr size_t SrvNormalChat::size() {
    size_t size = 0;
    size += sizeof(uint16_t);
    return size;
}

