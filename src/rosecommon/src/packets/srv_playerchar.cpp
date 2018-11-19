#include "srv_playerchar.h"
#include "throwassert.h"

namespace RoseCommon {

SrvPlayerChar::SrvPlayerChar() : CRosePacket(ePacketType::PAKWC_PLAYER_CHAR) {}

SrvPlayerChar::SrvPlayerChar(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_PLAYER_CHAR, "Not the right packet: " << to_underlying(get_type()));
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
	reader.get_uint8_t(race_);
	reader.get_uint16_t(runSpeed_);
	reader.get_uint16_t(atkSpeed_);
	reader.get_uint8_t(weightRate_);
	reader.get_uint32_t(face_);
	reader.get_uint32_t(hair_);
	for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) reader.get_uint32_t(inventory_[index]);
	for (size_t index = 0; index < BulletItems::BulletType::MAX_BULLET_TYPES; ++index) reader.get_uint16_t(bullets_[index]);
	reader.get_uint16_t(job_);
	reader.get_uint8_t(level_);
	for (size_t index = 0; index < RidingItems::RidingPosition::MAX_RIDING_ITEMS; ++index) reader.get_uint32_t(ridingItems_[index]);
	reader.get_uint16_t(z_);
	reader.get_uint32_t(subFlag_);
	reader.get_string(name_);
	reader.get_string(fakeName_);
}

SrvPlayerChar::SrvPlayerChar(uint16_t id, float x, float y, float destX, float destY, uint16_t command, uint16_t targetId, uint8_t moveMode, int32_t hp, int32_t teamId, uint32_t statusFlag, uint8_t race, uint16_t runSpeed, uint16_t atkSpeed, uint8_t weightRate, uint32_t face, uint32_t hair, uint32_t inventory[Inventory::maxVisibleEquippedItems], uint16_t bullets[BulletItems::BulletType::MAX_BULLET_TYPES], uint16_t job, uint8_t level, uint32_t ridingItems[RidingItems::RidingPosition::MAX_RIDING_ITEMS], uint16_t z, uint32_t subFlag, const std::string& name, const std::string& fakeName) : CRosePacket(ePacketType::PAKWC_PLAYER_CHAR), id_(id), x_(x), y_(y), destX_(destX), destY_(destY), command_(command), targetId_(targetId), moveMode_(moveMode), hp_(hp), teamId_(teamId), statusFlag_(statusFlag), race_(race), runSpeed_(runSpeed), atkSpeed_(atkSpeed), weightRate_(weightRate), face_(face), hair_(hair), job_(job), level_(level), z_(z), subFlag_(subFlag), name_(name), fakeName_(fakeName) {for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) inventory_[index] = inventory[index];
	for (size_t index = 0; index < BulletItems::BulletType::MAX_BULLET_TYPES; ++index) bullets_[index] = bullets[index];
	for (size_t index = 0; index < RidingItems::RidingPosition::MAX_RIDING_ITEMS; ++index) ridingItems_[index] = ridingItems[index];
}

uint16_t SrvPlayerChar::id() const { return id_; }

SrvPlayerChar& SrvPlayerChar::set_id(uint16_t data) { id_ = data; return *this; }

float SrvPlayerChar::x() const { return x_; }

SrvPlayerChar& SrvPlayerChar::set_x(float data) { x_ = data; return *this; }

float SrvPlayerChar::y() const { return y_; }

SrvPlayerChar& SrvPlayerChar::set_y(float data) { y_ = data; return *this; }

float SrvPlayerChar::destX() const { return destX_; }

SrvPlayerChar& SrvPlayerChar::set_destX(float data) { destX_ = data; return *this; }

float SrvPlayerChar::destY() const { return destY_; }

SrvPlayerChar& SrvPlayerChar::set_destY(float data) { destY_ = data; return *this; }

uint16_t SrvPlayerChar::command() const { return command_; }

SrvPlayerChar& SrvPlayerChar::set_command(uint16_t data) { command_ = data; return *this; }

uint16_t SrvPlayerChar::targetId() const { return targetId_; }

SrvPlayerChar& SrvPlayerChar::set_targetId(uint16_t data) { targetId_ = data; return *this; }

uint8_t SrvPlayerChar::moveMode() const { return moveMode_; }

SrvPlayerChar& SrvPlayerChar::set_moveMode(uint8_t data) { moveMode_ = data; return *this; }

int32_t SrvPlayerChar::hp() const { return hp_; }

SrvPlayerChar& SrvPlayerChar::set_hp(int32_t data) { hp_ = data; return *this; }

int32_t SrvPlayerChar::teamId() const { return teamId_; }

SrvPlayerChar& SrvPlayerChar::set_teamId(int32_t data) { teamId_ = data; return *this; }

uint32_t SrvPlayerChar::statusFlag() const { return statusFlag_; }

SrvPlayerChar& SrvPlayerChar::set_statusFlag(uint32_t data) { statusFlag_ = data; return *this; }

uint8_t SrvPlayerChar::race() const { return race_; }

SrvPlayerChar& SrvPlayerChar::set_race(uint8_t data) { race_ = data; return *this; }

uint16_t SrvPlayerChar::runSpeed() const { return runSpeed_; }

SrvPlayerChar& SrvPlayerChar::set_runSpeed(uint16_t data) { runSpeed_ = data; return *this; }

uint16_t SrvPlayerChar::atkSpeed() const { return atkSpeed_; }

SrvPlayerChar& SrvPlayerChar::set_atkSpeed(uint16_t data) { atkSpeed_ = data; return *this; }

uint8_t SrvPlayerChar::weightRate() const { return weightRate_; }

SrvPlayerChar& SrvPlayerChar::set_weightRate(uint8_t data) { weightRate_ = data; return *this; }

uint32_t SrvPlayerChar::face() const { return face_; }

SrvPlayerChar& SrvPlayerChar::set_face(uint32_t data) { face_ = data; return *this; }

uint32_t SrvPlayerChar::hair() const { return hair_; }

SrvPlayerChar& SrvPlayerChar::set_hair(uint32_t data) { hair_ = data; return *this; }

const uint32_t* SrvPlayerChar::inventory() const { return inventory_; }

const uint32_t& SrvPlayerChar::inventory(size_t index) const { return inventory_[index]; }

SrvPlayerChar& SrvPlayerChar::set_inventory(const uint32_t* data) { for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) inventory_[index] = data[index]; return *this; }

SrvPlayerChar& SrvPlayerChar::set_inventory(const uint32_t& data, size_t index) { inventory_[index] = data; return *this; }

const uint16_t* SrvPlayerChar::bullets() const { return bullets_; }

const uint16_t& SrvPlayerChar::bullets(size_t index) const { return bullets_[index]; }

SrvPlayerChar& SrvPlayerChar::set_bullets(const uint16_t* data) { for (size_t index = 0; index < BulletItems::BulletType::MAX_BULLET_TYPES; ++index) bullets_[index] = data[index]; return *this; }

SrvPlayerChar& SrvPlayerChar::set_bullets(const uint16_t& data, size_t index) { bullets_[index] = data; return *this; }

uint16_t SrvPlayerChar::job() const { return job_; }

SrvPlayerChar& SrvPlayerChar::set_job(uint16_t data) { job_ = data; return *this; }

uint8_t SrvPlayerChar::level() const { return level_; }

SrvPlayerChar& SrvPlayerChar::set_level(uint8_t data) { level_ = data; return *this; }

const uint32_t* SrvPlayerChar::ridingItems() const { return ridingItems_; }

const uint32_t& SrvPlayerChar::ridingItems(size_t index) const { return ridingItems_[index]; }

SrvPlayerChar& SrvPlayerChar::set_ridingItems(const uint32_t* data) { for (size_t index = 0; index < RidingItems::RidingPosition::MAX_RIDING_ITEMS; ++index) ridingItems_[index] = data[index]; return *this; }

SrvPlayerChar& SrvPlayerChar::set_ridingItems(const uint32_t& data, size_t index) { ridingItems_[index] = data; return *this; }

uint16_t SrvPlayerChar::z() const { return z_; }

SrvPlayerChar& SrvPlayerChar::set_z(uint16_t data) { z_ = data; return *this; }

uint32_t SrvPlayerChar::subFlag() const { return subFlag_; }

SrvPlayerChar& SrvPlayerChar::set_subFlag(uint32_t data) { subFlag_ = data; return *this; }

const std::string& SrvPlayerChar::name() const { return name_; }

SrvPlayerChar& SrvPlayerChar::set_name(const std::string& data) { name_ = data; return *this; }

const std::string& SrvPlayerChar::fakeName() const { return fakeName_; }

SrvPlayerChar& SrvPlayerChar::set_fakeName(const std::string& data) { fakeName_ = data; return *this; }


void SrvPlayerChar::pack(CRoseBasePolicy& writer) const {
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
	writer.set_uint8_t(race_);
	writer.set_uint16_t(runSpeed_);
	writer.set_uint16_t(atkSpeed_);
	writer.set_uint8_t(weightRate_);
	writer.set_uint32_t(face_);
	writer.set_uint32_t(hair_);
	for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) writer.set_uint32_t(inventory_[index]);
	for (size_t index = 0; index < BulletItems::BulletType::MAX_BULLET_TYPES; ++index) writer.set_uint16_t(bullets_[index]);
	writer.set_uint16_t(job_);
	writer.set_uint8_t(level_);
	for (size_t index = 0; index < RidingItems::RidingPosition::MAX_RIDING_ITEMS; ++index) writer.set_uint32_t(ridingItems_[index]);
	writer.set_uint16_t(z_);
	writer.set_uint32_t(subFlag_);
	writer.set_string(name_);
	writer.set_string(fakeName_);
}

SrvPlayerChar SrvPlayerChar::create(Entity entity) {
	const auto entity_ridingitems = entity.component<RidingItems>();
	const auto entity_characterinfo = entity.component<CharacterInfo>();
	const auto entity_charactergraphics = entity.component<CharacterGraphics>();
	const auto entity_position = entity.component<Position>();
	const auto entity_basicinfo = entity.component<BasicInfo>();
	const auto entity_inventory = entity.component<Inventory>();
	const auto entity_advancedinfo = entity.component<AdvancedInfo>();
	const auto entity_bulletitems = entity.component<BulletItems>();
	uint32_t inventory_[Inventory::maxVisibleEquippedItems];
	for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) inventory_[index] = entity_inventory->getEquipped()[index].getVisible();
	uint16_t bullets_[BulletItems::BulletType::MAX_BULLET_TYPES];
	for (size_t index = 0; index < BulletItems::BulletType::MAX_BULLET_TYPES; ++index) bullets_[index] = entity_bulletitems->items_[index].getHeader();
	uint32_t ridingItems_[RidingItems::RidingPosition::MAX_RIDING_ITEMS];
	for (size_t index = 0; index < RidingItems::RidingPosition::MAX_RIDING_ITEMS; ++index) ridingItems_[index] = entity_ridingitems->items_[index].getVisible();

	return SrvPlayerChar(entity_basicinfo->id_, entity_position->x_, entity_position->y_, entity_position->x_, entity_position->y_, entity_basicinfo->command_, entity_basicinfo->targetId_, entity_advancedinfo->moveMode_, entity_advancedinfo->hp_, entity_basicinfo->teamId_, entity_characterinfo->statusFlag_, entity_charactergraphics->race_, entity_advancedinfo->runSpeed_, entity_advancedinfo->atkSpeed_, entity_advancedinfo->weightRate_, entity_charactergraphics->face_, entity_charactergraphics->hair_, inventory_, bullets_, entity_characterinfo->job_, entity_basicinfo->level_, ridingItems_, entity_position->z_, entity_characterinfo->subFlag_, entity_basicinfo->name_, entity_basicinfo->name_);
}

SrvPlayerChar SrvPlayerChar::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvPlayerChar(reader);
}
std::unique_ptr<SrvPlayerChar> SrvPlayerChar::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvPlayerChar>(reader);
}

}