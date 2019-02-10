#include "srv_npc_char.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvNpcChar::SrvNpcChar() : CRosePacket(SrvNpcChar::PACKET_ID) {}

SrvNpcChar::SrvNpcChar(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(id)) {
        return;
    }
    if (!reader.get_float(x)) {
        return;
    }
    if (!reader.get_float(y)) {
        return;
    }
    if (!reader.get_float(destX)) {
        return;
    }
    if (!reader.get_float(destY)) {
        return;
    }
    if (!reader.get_uint16_t(command)) {
        return;
    }
    if (!reader.get_uint16_t(targetId)) {
        return;
    }
    if (!reader.get_uint8_t(moveMode)) {
        return;
    }
    if (!reader.get_int32_t(hp)) {
        return;
    }
    if (!reader.get_int32_t(teamId)) {
        return;
    }
    if (!reader.get_uint32_t(statusFlag)) {
        return;
    }
    if (!reader.get_uint16_t(npcId)) {
        return;
    }
    if (!reader.get_uint16_t(questId)) {
        return;
    }
    if (!reader.get_float(angle)) {
        return;
    }
    if (!reader.get_uint16_t(eventStatus)) {
        return;
    }
}

void SrvNpcChar::set_id(const uint16_t id) {
    this->id = id;
}

uint16_t SrvNpcChar::get_id() const {
    return id;
}

void SrvNpcChar::set_x(const float x) {
    this->x = x;
}

float SrvNpcChar::get_x() const {
    return x;
}

void SrvNpcChar::set_y(const float y) {
    this->y = y;
}

float SrvNpcChar::get_y() const {
    return y;
}

void SrvNpcChar::set_destX(const float destX) {
    this->destX = destX;
}

float SrvNpcChar::get_destX() const {
    return destX;
}

void SrvNpcChar::set_destY(const float destY) {
    this->destY = destY;
}

float SrvNpcChar::get_destY() const {
    return destY;
}

void SrvNpcChar::set_command(const uint16_t command) {
    this->command = command;
}

uint16_t SrvNpcChar::get_command() const {
    return command;
}

void SrvNpcChar::set_targetId(const uint16_t targetId) {
    this->targetId = targetId;
}

uint16_t SrvNpcChar::get_targetId() const {
    return targetId;
}

void SrvNpcChar::set_moveMode(const uint8_t moveMode) {
    this->moveMode = moveMode;
}

uint8_t SrvNpcChar::get_moveMode() const {
    return moveMode;
}

void SrvNpcChar::set_hp(const int32_t hp) {
    this->hp = hp;
}

int32_t SrvNpcChar::get_hp() const {
    return hp;
}

void SrvNpcChar::set_teamId(const int32_t teamId) {
    this->teamId = teamId;
}

int32_t SrvNpcChar::get_teamId() const {
    return teamId;
}

void SrvNpcChar::set_statusFlag(const uint32_t statusFlag) {
    this->statusFlag = statusFlag;
}

uint32_t SrvNpcChar::get_statusFlag() const {
    return statusFlag;
}

void SrvNpcChar::set_npcId(const uint16_t npcId) {
    this->npcId = npcId;
}

uint16_t SrvNpcChar::get_npcId() const {
    return npcId;
}

void SrvNpcChar::set_questId(const uint16_t questId) {
    this->questId = questId;
}

uint16_t SrvNpcChar::get_questId() const {
    return questId;
}

void SrvNpcChar::set_angle(const float angle) {
    this->angle = angle;
}

float SrvNpcChar::get_angle() const {
    return angle;
}

void SrvNpcChar::set_eventStatus(const uint16_t eventStatus) {
    this->eventStatus = eventStatus;
}

uint16_t SrvNpcChar::get_eventStatus() const {
    return eventStatus;
}

SrvNpcChar SrvNpcChar::create(const uint16_t& id) {
    SrvNpcChar packet;
    packet.set_id(id);
    return packet;
}

SrvNpcChar SrvNpcChar::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvNpcChar(reader);
}

std::unique_ptr<SrvNpcChar> SrvNpcChar::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvNpcChar>(reader);
}

bool SrvNpcChar::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(id)) {
        return false;
    }
    if (!writer.set_float(x)) {
        return false;
    }
    if (!writer.set_float(y)) {
        return false;
    }
    if (!writer.set_float(destX)) {
        return false;
    }
    if (!writer.set_float(destY)) {
        return false;
    }
    if (!writer.set_uint16_t(command)) {
        return false;
    }
    if (!writer.set_uint16_t(targetId)) {
        return false;
    }
    if (!writer.set_uint8_t(moveMode)) {
        return false;
    }
    if (!writer.set_int32_t(hp)) {
        return false;
    }
    if (!writer.set_int32_t(teamId)) {
        return false;
    }
    if (!writer.set_uint32_t(statusFlag)) {
        return false;
    }
    if (!writer.set_uint16_t(npcId)) {
        return false;
    }
    if (!writer.set_uint16_t(questId)) {
        return false;
    }
    if (!writer.set_float(angle)) {
        return false;
    }
    if (!writer.set_uint16_t(eventStatus)) {
        return false;
    }
    return true;
}

constexpr size_t SrvNpcChar::size() {
    size_t size = 0;
    size += sizeof(uint16_t); // id
    size += sizeof(float); // x
    size += sizeof(float); // y
    size += sizeof(float); // destX
    size += sizeof(float); // destY
    size += sizeof(uint16_t); // command
    size += sizeof(uint16_t); // targetId
    size += sizeof(uint8_t); // moveMode
    size += sizeof(int32_t); // hp
    size += sizeof(int32_t); // teamId
    size += sizeof(uint32_t); // statusFlag
    size += sizeof(uint16_t); // npcId
    size += sizeof(uint16_t); // questId
    size += sizeof(float); // angle
    size += sizeof(uint16_t); // eventStatus
    return size;
}

