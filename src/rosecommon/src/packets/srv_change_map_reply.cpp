#include "srv_change_map_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvChangeMapReply::SrvChangeMapReply() : CRosePacket(SrvChangeMapReply::PACKET_ID) {}

SrvChangeMapReply::SrvChangeMapReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(objectIndex)) {
        return;
    }
    if (!reader.get_uint16_t(hp)) {
        return;
    }
    if (!reader.get_uint16_t(mp)) {
        return;
    }
    if (!reader.get_uint16_t(xp)) {
        return;
    }
    if (!reader.get_uint16_t(penalizeXp)) {
        return;
    }
    if (!reader.get_uint16_t(craftRate)) {
        return;
    }
    if (!reader.get_uint16_t(updateTime)) {
        return;
    }
    if (!reader.get_uint16_t(worldRate)) {
        return;
    }
    if (!reader.get_uint8_t(townRate)) {
        return;
    }
    for (size_t index = 0; index < MAX_SELL_TYPE; ++index) {
        if (!reader.get_uint8_t(itemRate[index])) {
            return;
        }
    }
    if (!reader.get_uint32_t(flags)) {
        return;
    }
    if (!reader.get_uint16_t(worldTime)) {
        return;
    }
    if (!reader.get_uint16_t(teamNumber)) {
        return;
    }
}

void SrvChangeMapReply::set_objectIndex(const uint16_t objectIndex) {
    this->objectIndex = objectIndex;
}

uint16_t SrvChangeMapReply::get_objectIndex() const {
    return objectIndex;
}

void SrvChangeMapReply::set_hp(const uint16_t hp) {
    this->hp = hp;
}

uint16_t SrvChangeMapReply::get_hp() const {
    return hp;
}

void SrvChangeMapReply::set_mp(const uint16_t mp) {
    this->mp = mp;
}

uint16_t SrvChangeMapReply::get_mp() const {
    return mp;
}

void SrvChangeMapReply::set_xp(const uint16_t xp) {
    this->xp = xp;
}

uint16_t SrvChangeMapReply::get_xp() const {
    return xp;
}

void SrvChangeMapReply::set_penalizeXp(const uint16_t penalizeXp) {
    this->penalizeXp = penalizeXp;
}

uint16_t SrvChangeMapReply::get_penalizeXp() const {
    return penalizeXp;
}

void SrvChangeMapReply::set_craftRate(const uint16_t craftRate) {
    this->craftRate = craftRate;
}

uint16_t SrvChangeMapReply::get_craftRate() const {
    return craftRate;
}

void SrvChangeMapReply::set_updateTime(const uint16_t updateTime) {
    this->updateTime = updateTime;
}

uint16_t SrvChangeMapReply::get_updateTime() const {
    return updateTime;
}

void SrvChangeMapReply::set_worldRate(const uint16_t worldRate) {
    this->worldRate = worldRate;
}

uint16_t SrvChangeMapReply::get_worldRate() const {
    return worldRate;
}

void SrvChangeMapReply::set_townRate(const uint8_t townRate) {
    this->townRate = townRate;
}

uint8_t SrvChangeMapReply::get_townRate() const {
    return townRate;
}

void SrvChangeMapReply::set_itemRate(const std::array<uint8_t, MAX_SELL_TYPE> itemRate) {
    this->itemRate = itemRate;
}

void SrvChangeMapReply::set_itemRate(const uint8_t itemRate, size_t index) {
    this->itemRate[index] = itemRate;
}

std::array<uint8_t, MAX_SELL_TYPE> SrvChangeMapReply::get_itemRate() const {
    return itemRate;
}

uint8_t SrvChangeMapReply::get_itemRate(size_t index) const {
    return itemRate[index];
}

void SrvChangeMapReply::set_flags(const uint32_t flags) {
    this->flags = flags;
}

uint32_t SrvChangeMapReply::get_flags() const {
    return flags;
}

void SrvChangeMapReply::set_worldTime(const uint16_t worldTime) {
    this->worldTime = worldTime;
}

uint16_t SrvChangeMapReply::get_worldTime() const {
    return worldTime;
}

void SrvChangeMapReply::set_teamNumber(const uint16_t teamNumber) {
    this->teamNumber = teamNumber;
}

uint16_t SrvChangeMapReply::get_teamNumber() const {
    return teamNumber;
}

SrvChangeMapReply SrvChangeMapReply::create(const uint16_t& objectIndex, const uint16_t& hp, const uint16_t& mp, const uint16_t& xp, const uint16_t& penalizeXp, const uint16_t& worldTime, const uint16_t& teamNumber) {
    SrvChangeMapReply packet;
    packet.set_objectIndex(objectIndex);
    packet.set_hp(hp);
    packet.set_mp(mp);
    packet.set_xp(xp);
    packet.set_penalizeXp(penalizeXp);
    packet.set_worldTime(worldTime);
    packet.set_teamNumber(teamNumber);
    return packet;
}

SrvChangeMapReply SrvChangeMapReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvChangeMapReply(reader);
}

std::unique_ptr<SrvChangeMapReply> SrvChangeMapReply::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvChangeMapReply>(reader);
}

void SrvChangeMapReply::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(objectIndex)) {
        return;
    }
    if (!writer.set_uint16_t(hp)) {
        return;
    }
    if (!writer.set_uint16_t(mp)) {
        return;
    }
    if (!writer.set_uint16_t(xp)) {
        return;
    }
    if (!writer.set_uint16_t(penalizeXp)) {
        return;
    }
    if (!writer.set_uint16_t(craftRate)) {
        return;
    }
    if (!writer.set_uint16_t(updateTime)) {
        return;
    }
    if (!writer.set_uint16_t(worldRate)) {
        return;
    }
    if (!writer.set_uint8_t(townRate)) {
        return;
    }
    for (const auto& elem : itemRate) {
        if (!writer.set_uint8_t(elem)) {
            return;
        }
    }
    if (!writer.set_uint32_t(flags)) {
        return;
    }
    if (!writer.set_uint16_t(worldTime)) {
        return;
    }
    if (!writer.set_uint16_t(teamNumber)) {
        return;
    }
}

constexpr size_t SrvChangeMapReply::size() {
    size_t size = 0;
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += sizeof(uint8_t);
    size += sizeof(uint8_t) * MAX_SELL_TYPE;
    size += sizeof(uint32_t);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    return size;
}

