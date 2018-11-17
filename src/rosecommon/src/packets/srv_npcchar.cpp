#include "srv_npcchar.h"
#include "throwassert.h"

namespace RoseCommon {

SrvNpcChar::SrvNpcChar() : CRosePacket(ePacketType::PAKWC_NPC_CHAR) {}

SrvNpcChar::SrvNpcChar(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_NPC_CHAR, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_float(destX_);
	reader.get_float(destY_);
	reader.get_uint16_t(command_);
	reader.get_uint16_t(targetId_);
	reader.get_uint8_t(moveMode_);
	reader.get_int32_t(hp_);
	reader.get_int32_t(teamId_);
	reader.get_uint32_t(statusFlag_);
	reader.get_uint16_t(npcId_);
	reader.get_uint16_t(questId_);
	reader.get_float(angle_);
	reader.get_uint16_t(eventStatus_);
}

SrvNpcChar::SrvNpcChar(uint16_t id, float x, float y, float destX, float destY, uint16_t command, uint16_t targetId, uint8_t moveMode, int32_t hp, int32_t teamId, uint32_t statusFlag, uint16_t npcId, uint16_t questId, float angle) : CRosePacket(ePacketType::PAKWC_NPC_CHAR), id_(id), x_(x), y_(y), destX_(destX), destY_(destY), command_(command), targetId_(targetId), moveMode_(moveMode), hp_(hp), teamId_(teamId), statusFlag_(statusFlag), npcId_(npcId), questId_(questId), angle_(angle) {
}

uint16_t SrvNpcChar::id() const { return id_; }

SrvNpcChar& SrvNpcChar::set_id(uint16_t data) { id_ = data; return *this; }

float SrvNpcChar::x() const { return x_; }

SrvNpcChar& SrvNpcChar::set_x(float data) { x_ = data; return *this; }

float SrvNpcChar::y() const { return y_; }

SrvNpcChar& SrvNpcChar::set_y(float data) { y_ = data; return *this; }

float SrvNpcChar::destX() const { return destX_; }

SrvNpcChar& SrvNpcChar::set_destX(float data) { destX_ = data; return *this; }

float SrvNpcChar::destY() const { return destY_; }

SrvNpcChar& SrvNpcChar::set_destY(float data) { destY_ = data; return *this; }

uint16_t SrvNpcChar::command() const { return command_; }

SrvNpcChar& SrvNpcChar::set_command(uint16_t data) { command_ = data; return *this; }

uint16_t SrvNpcChar::targetId() const { return targetId_; }

SrvNpcChar& SrvNpcChar::set_targetId(uint16_t data) { targetId_ = data; return *this; }

uint8_t SrvNpcChar::moveMode() const { return moveMode_; }

SrvNpcChar& SrvNpcChar::set_moveMode(uint8_t data) { moveMode_ = data; return *this; }

int32_t SrvNpcChar::hp() const { return hp_; }

SrvNpcChar& SrvNpcChar::set_hp(int32_t data) { hp_ = data; return *this; }

int32_t SrvNpcChar::teamId() const { return teamId_; }

SrvNpcChar& SrvNpcChar::set_teamId(int32_t data) { teamId_ = data; return *this; }

uint32_t SrvNpcChar::statusFlag() const { return statusFlag_; }

SrvNpcChar& SrvNpcChar::set_statusFlag(uint32_t data) { statusFlag_ = data; return *this; }

uint16_t SrvNpcChar::npcId() const { return npcId_; }

SrvNpcChar& SrvNpcChar::set_npcId(uint16_t data) { npcId_ = data; return *this; }

uint16_t SrvNpcChar::questId() const { return questId_; }

SrvNpcChar& SrvNpcChar::set_questId(uint16_t data) { questId_ = data; return *this; }

float SrvNpcChar::angle() const { return angle_; }

SrvNpcChar& SrvNpcChar::set_angle(float data) { angle_ = data; return *this; }

uint16_t SrvNpcChar::eventStatus() const { return eventStatus_; }

SrvNpcChar& SrvNpcChar::set_eventStatus(uint16_t data) { eventStatus_ = data; return *this; }


void SrvNpcChar::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(id_);
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_float(destX_);
	writer.set_float(destY_);
	writer.set_uint16_t(command_);
	writer.set_uint16_t(targetId_);
	writer.set_uint8_t(moveMode_);
	writer.set_int32_t(hp_);
	writer.set_int32_t(teamId_);
	writer.set_uint32_t(statusFlag_);
	writer.set_uint16_t(npcId_);
	writer.set_uint16_t(questId_);
	writer.set_float(angle_);
	writer.set_uint16_t(eventStatus_);
}

uint16_t SrvNpcChar::get_size() const {
	uint16_t size = 0;
	size += sizeof(id_);
	size += sizeof(x_);
	size += sizeof(y_);
	size += sizeof(destX_);
	size += sizeof(destY_);
	size += sizeof(command_);
	size += sizeof(targetId_);
	size += sizeof(moveMode_);
	size += sizeof(hp_);
	size += sizeof(teamId_);
	size += sizeof(statusFlag_);
	size += sizeof(npcId_);
	size += sizeof(questId_);
	size += sizeof(angle_);
	size += sizeof(eventStatus_);
	return size;
}


SrvNpcChar SrvNpcChar::create(Entity entity) {
	const auto entity_characterinfo = entity.component<CharacterInfo>();
	const auto entity_npc = entity.component<Npc>();
	const auto entity_position = entity.component<Position>();
	const auto entity_destination = entity.component<Destination>();
	const auto entity_basicinfo = entity.component<BasicInfo>();
	const auto entity_advancedinfo = entity.component<AdvancedInfo>();

	return SrvNpcChar(entity_basicinfo->id_, entity_position->x_, entity_position->y_, entity_destination ? entity_destination->x_ : entity_position->x_, entity_destination ? entity_destination->y_ : entity_position->y_, entity_basicinfo->command_, entity_basicinfo->targetId_, entity_advancedinfo->moveMode_, entity_advancedinfo->hp_, entity_basicinfo->teamId_, entity_characterinfo->statusFlag_, entity_npc->npc_id_, entity_npc->quest_id_, entity_position->angle_);
}

SrvNpcChar SrvNpcChar::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvNpcChar(reader);
}

}