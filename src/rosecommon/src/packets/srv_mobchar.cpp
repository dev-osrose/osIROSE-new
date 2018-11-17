#include "srv_mobchar.h"
#include "throwassert.h"

namespace RoseCommon {

SrvMobChar::SrvMobChar() : CRosePacket(ePacketType::PAKWC_MOB_CHAR) {}

SrvMobChar::SrvMobChar(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_MOB_CHAR, "Not the right packet: " << to_underlying(get_type()));
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
}

SrvMobChar::SrvMobChar(uint16_t id, float x, float y, float destX, float destY, uint16_t command, uint16_t targetId, uint8_t moveMode, int32_t hp, int32_t teamId, uint32_t statusFlag, uint16_t npcId, uint16_t questId) : CRosePacket(ePacketType::PAKWC_MOB_CHAR), id_(id), x_(x), y_(y), destX_(destX), destY_(destY), command_(command), targetId_(targetId), moveMode_(moveMode), hp_(hp), teamId_(teamId), statusFlag_(statusFlag), npcId_(npcId), questId_(questId) {
}

uint16_t SrvMobChar::id() const { return id_; }

SrvMobChar& SrvMobChar::set_id(uint16_t data) { id_ = data; return *this; }

float SrvMobChar::x() const { return x_; }

SrvMobChar& SrvMobChar::set_x(float data) { x_ = data; return *this; }

float SrvMobChar::y() const { return y_; }

SrvMobChar& SrvMobChar::set_y(float data) { y_ = data; return *this; }

float SrvMobChar::destX() const { return destX_; }

SrvMobChar& SrvMobChar::set_destX(float data) { destX_ = data; return *this; }

float SrvMobChar::destY() const { return destY_; }

SrvMobChar& SrvMobChar::set_destY(float data) { destY_ = data; return *this; }

uint16_t SrvMobChar::command() const { return command_; }

SrvMobChar& SrvMobChar::set_command(uint16_t data) { command_ = data; return *this; }

uint16_t SrvMobChar::targetId() const { return targetId_; }

SrvMobChar& SrvMobChar::set_targetId(uint16_t data) { targetId_ = data; return *this; }

uint8_t SrvMobChar::moveMode() const { return moveMode_; }

SrvMobChar& SrvMobChar::set_moveMode(uint8_t data) { moveMode_ = data; return *this; }

int32_t SrvMobChar::hp() const { return hp_; }

SrvMobChar& SrvMobChar::set_hp(int32_t data) { hp_ = data; return *this; }

int32_t SrvMobChar::teamId() const { return teamId_; }

SrvMobChar& SrvMobChar::set_teamId(int32_t data) { teamId_ = data; return *this; }

uint32_t SrvMobChar::statusFlag() const { return statusFlag_; }

SrvMobChar& SrvMobChar::set_statusFlag(uint32_t data) { statusFlag_ = data; return *this; }

uint16_t SrvMobChar::npcId() const { return npcId_; }

SrvMobChar& SrvMobChar::set_npcId(uint16_t data) { npcId_ = data; return *this; }

uint16_t SrvMobChar::questId() const { return questId_; }

SrvMobChar& SrvMobChar::set_questId(uint16_t data) { questId_ = data; return *this; }


void SrvMobChar::pack(CRoseWriter& writer) const {
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
}

uint16_t SrvMobChar::get_size() const {
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
	return size;
}


SrvMobChar SrvMobChar::create(Entity entity) {
	const auto entity_characterinfo = entity.component<CharacterInfo>();
	const auto entity_npc = entity.component<Npc>();
	const auto entity_position = entity.component<Position>();
	const auto entity_destination = entity.component<Destination>();
	const auto entity_basicinfo = entity.component<BasicInfo>();
	const auto entity_advancedinfo = entity.component<AdvancedInfo>();

	return SrvMobChar(entity_basicinfo->id_, entity_position->x_, entity_position->y_, entity_destination ? entity_destination->x_ : entity_position->x_, entity_destination ? entity_destination->y_ : entity_position->y_, entity_basicinfo->command_, entity_basicinfo->targetId_, entity_advancedinfo->moveMode_, entity_advancedinfo->hp_, entity_basicinfo->teamId_, entity_characterinfo->statusFlag_, entity_npc->npc_id_, entity_npc->quest_id_);
}

SrvMobChar SrvMobChar::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvMobChar(reader);
}

}