#include "srv_selectcharreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvSelectCharReply::SrvSelectCharReply() : CRosePacket(ePacketType::PAKWC_SELECT_CHAR_REPLY) {}

SrvSelectCharReply::SrvSelectCharReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_SELECT_CHAR_REPLY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint8_t(race_);
	reader.get_uint16_t(map_);
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_uint16_t(spawn_);
	reader.get_uint32_t(mask_);
	reader.get_uint32_t(headGear_);
	for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) reader.get_uint32_t(inventory_[index]);
	reader.get_uint8_t(stone_);
	reader.get_uint8_t(face_);
	reader.get_uint8_t(hair_);
	reader.get_uint16_t(job_);
	reader.get_uint8_t(factionId_);
	reader.get_uint8_t(factionRank_);
	reader.get_uint8_t(fame_);
	reader.get_uint16_t(str_);
	reader.get_uint16_t(dex_);
	reader.get_uint16_t(intel_);
	reader.get_uint16_t(con_);
	reader.get_uint16_t(charm_);
	reader.get_uint16_t(sense_);
	reader.get_int32_t(hp_);
	reader.get_int32_t(mp_);
	reader.get_uint32_t(xp_);
	reader.get_uint16_t(level_);
	reader.get_uint32_t(statPoints_);
	reader.get_uint32_t(skillPoints_);
	reader.get_uint8_t(bodySize_);
	reader.get_uint8_t(headSize_);
	reader.get_uint32_t(penaltyXp_);
	for (size_t index = 0; index < 2; ++index) reader.get_uint16_t(factionFame_[index]);
	for (size_t index = 0; index < 10; ++index) reader.get_uint16_t(factionPoints_[index]);
	reader.get_uint32_t(guildId_);
	reader.get_uint16_t(guildContribution_);
	reader.get_uint8_t(guildRank_);
	reader.get_uint16_t(pkFlag_);
	reader.get_uint16_t(stamina_);
	for (size_t index = 0; index < StatusEffects::maxEffects; ++index) reader.get_iserialize(effects_[index]);
	reader.get_uint16_t(patHp_);
	reader.get_uint32_t(patCooldownTime_);
	for (size_t index = 0; index < Skills::maxSkills; ++index) reader.get_uint16_t(skills_[index]);
	for (size_t index = 0; index < Hotbar::maxItems; ++index) reader.get_uint16_t(hotbar_[index]);
	reader.get_uint32_t(tag_);
	reader.get_string(name_);
}

SrvSelectCharReply::SrvSelectCharReply(uint8_t race, uint16_t map, float x, float y, uint16_t spawn, uint32_t mask, uint32_t headGear, uint32_t inventory[Inventory::maxVisibleEquippedItems], uint8_t stone, uint8_t face, uint8_t hair, uint16_t job, uint8_t factionId, uint8_t factionRank, uint8_t fame, uint16_t str, uint16_t dex, uint16_t intel, uint16_t con, uint16_t charm, uint16_t sense, int32_t hp, int32_t mp, uint32_t xp, uint16_t level, uint32_t statPoints, uint32_t skillPoints, uint8_t bodySize, uint8_t headSize, uint32_t penaltyXp, uint16_t factionFame[2], uint16_t factionPoints[10], uint32_t guildId, uint16_t guildContribution, uint8_t guildRank, uint16_t pkFlag, uint16_t stamina, StatusEffect effects[StatusEffects::maxEffects], uint16_t patHp, uint32_t patCooldownTime, uint16_t skills[Skills::maxSkills], uint16_t hotbar[Hotbar::maxItems], uint32_t tag, const std::string& name) : CRosePacket(ePacketType::PAKWC_SELECT_CHAR_REPLY), race_(race), map_(map), x_(x), y_(y), spawn_(spawn), mask_(mask), headGear_(headGear), stone_(stone), face_(face), hair_(hair), job_(job), factionId_(factionId), factionRank_(factionRank), fame_(fame), str_(str), dex_(dex), intel_(intel), con_(con), charm_(charm), sense_(sense), hp_(hp), mp_(mp), xp_(xp), level_(level), statPoints_(statPoints), skillPoints_(skillPoints), bodySize_(bodySize), headSize_(headSize), penaltyXp_(penaltyXp), guildId_(guildId), guildContribution_(guildContribution), guildRank_(guildRank), pkFlag_(pkFlag), stamina_(stamina), patHp_(patHp), patCooldownTime_(patCooldownTime), tag_(tag), name_(name) {for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) inventory_[index] = inventory[index];
	for (size_t index = 0; index < 2; ++index) factionFame_[index] = factionFame[index];
	for (size_t index = 0; index < 10; ++index) factionPoints_[index] = factionPoints[index];
	for (size_t index = 0; index < StatusEffects::maxEffects; ++index) effects_[index] = effects[index];
	for (size_t index = 0; index < Skills::maxSkills; ++index) skills_[index] = skills[index];
	for (size_t index = 0; index < Hotbar::maxItems; ++index) hotbar_[index] = hotbar[index];
}

uint8_t SrvSelectCharReply::race() const { return race_; }

SrvSelectCharReply& SrvSelectCharReply::set_race(uint8_t data) { race_ = data; return *this; }

uint16_t SrvSelectCharReply::map() const { return map_; }

SrvSelectCharReply& SrvSelectCharReply::set_map(uint16_t data) { map_ = data; return *this; }

float SrvSelectCharReply::x() const { return x_; }

SrvSelectCharReply& SrvSelectCharReply::set_x(float data) { x_ = data; return *this; }

float SrvSelectCharReply::y() const { return y_; }

SrvSelectCharReply& SrvSelectCharReply::set_y(float data) { y_ = data; return *this; }

uint16_t SrvSelectCharReply::spawn() const { return spawn_; }

SrvSelectCharReply& SrvSelectCharReply::set_spawn(uint16_t data) { spawn_ = data; return *this; }

uint32_t SrvSelectCharReply::mask() const { return mask_; }

SrvSelectCharReply& SrvSelectCharReply::set_mask(uint32_t data) { mask_ = data; return *this; }

uint32_t SrvSelectCharReply::headGear() const { return headGear_; }

SrvSelectCharReply& SrvSelectCharReply::set_headGear(uint32_t data) { headGear_ = data; return *this; }

const uint32_t* SrvSelectCharReply::inventory() const { return inventory_; }

const uint32_t& SrvSelectCharReply::inventory(size_t index) const { return inventory_[index]; }

SrvSelectCharReply& SrvSelectCharReply::set_inventory(const uint32_t* data) { for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) inventory_[index] = data[index]; return *this; }

SrvSelectCharReply& SrvSelectCharReply::set_inventory(const uint32_t& data, size_t index) { inventory_[index] = data; return *this; }

uint8_t SrvSelectCharReply::stone() const { return stone_; }

SrvSelectCharReply& SrvSelectCharReply::set_stone(uint8_t data) { stone_ = data; return *this; }

uint8_t SrvSelectCharReply::face() const { return face_; }

SrvSelectCharReply& SrvSelectCharReply::set_face(uint8_t data) { face_ = data; return *this; }

uint8_t SrvSelectCharReply::hair() const { return hair_; }

SrvSelectCharReply& SrvSelectCharReply::set_hair(uint8_t data) { hair_ = data; return *this; }

uint16_t SrvSelectCharReply::job() const { return job_; }

SrvSelectCharReply& SrvSelectCharReply::set_job(uint16_t data) { job_ = data; return *this; }

uint8_t SrvSelectCharReply::factionId() const { return factionId_; }

SrvSelectCharReply& SrvSelectCharReply::set_factionId(uint8_t data) { factionId_ = data; return *this; }

uint8_t SrvSelectCharReply::factionRank() const { return factionRank_; }

SrvSelectCharReply& SrvSelectCharReply::set_factionRank(uint8_t data) { factionRank_ = data; return *this; }

uint8_t SrvSelectCharReply::fame() const { return fame_; }

SrvSelectCharReply& SrvSelectCharReply::set_fame(uint8_t data) { fame_ = data; return *this; }

uint16_t SrvSelectCharReply::str() const { return str_; }

SrvSelectCharReply& SrvSelectCharReply::set_str(uint16_t data) { str_ = data; return *this; }

uint16_t SrvSelectCharReply::dex() const { return dex_; }

SrvSelectCharReply& SrvSelectCharReply::set_dex(uint16_t data) { dex_ = data; return *this; }

uint16_t SrvSelectCharReply::intel() const { return intel_; }

SrvSelectCharReply& SrvSelectCharReply::set_intel(uint16_t data) { intel_ = data; return *this; }

uint16_t SrvSelectCharReply::con() const { return con_; }

SrvSelectCharReply& SrvSelectCharReply::set_con(uint16_t data) { con_ = data; return *this; }

uint16_t SrvSelectCharReply::charm() const { return charm_; }

SrvSelectCharReply& SrvSelectCharReply::set_charm(uint16_t data) { charm_ = data; return *this; }

uint16_t SrvSelectCharReply::sense() const { return sense_; }

SrvSelectCharReply& SrvSelectCharReply::set_sense(uint16_t data) { sense_ = data; return *this; }

int32_t SrvSelectCharReply::hp() const { return hp_; }

SrvSelectCharReply& SrvSelectCharReply::set_hp(int32_t data) { hp_ = data; return *this; }

int32_t SrvSelectCharReply::mp() const { return mp_; }

SrvSelectCharReply& SrvSelectCharReply::set_mp(int32_t data) { mp_ = data; return *this; }

uint32_t SrvSelectCharReply::xp() const { return xp_; }

SrvSelectCharReply& SrvSelectCharReply::set_xp(uint32_t data) { xp_ = data; return *this; }

uint16_t SrvSelectCharReply::level() const { return level_; }

SrvSelectCharReply& SrvSelectCharReply::set_level(uint16_t data) { level_ = data; return *this; }

uint32_t SrvSelectCharReply::statPoints() const { return statPoints_; }

SrvSelectCharReply& SrvSelectCharReply::set_statPoints(uint32_t data) { statPoints_ = data; return *this; }

uint32_t SrvSelectCharReply::skillPoints() const { return skillPoints_; }

SrvSelectCharReply& SrvSelectCharReply::set_skillPoints(uint32_t data) { skillPoints_ = data; return *this; }

uint8_t SrvSelectCharReply::bodySize() const { return bodySize_; }

SrvSelectCharReply& SrvSelectCharReply::set_bodySize(uint8_t data) { bodySize_ = data; return *this; }

uint8_t SrvSelectCharReply::headSize() const { return headSize_; }

SrvSelectCharReply& SrvSelectCharReply::set_headSize(uint8_t data) { headSize_ = data; return *this; }

uint32_t SrvSelectCharReply::penaltyXp() const { return penaltyXp_; }

SrvSelectCharReply& SrvSelectCharReply::set_penaltyXp(uint32_t data) { penaltyXp_ = data; return *this; }

const uint16_t* SrvSelectCharReply::factionFame() const { return factionFame_; }

const uint16_t& SrvSelectCharReply::factionFame(size_t index) const { return factionFame_[index]; }

SrvSelectCharReply& SrvSelectCharReply::set_factionFame(const uint16_t* data) { for (size_t index = 0; index < 2; ++index) factionFame_[index] = data[index]; return *this; }

SrvSelectCharReply& SrvSelectCharReply::set_factionFame(const uint16_t& data, size_t index) { factionFame_[index] = data; return *this; }

const uint16_t* SrvSelectCharReply::factionPoints() const { return factionPoints_; }

const uint16_t& SrvSelectCharReply::factionPoints(size_t index) const { return factionPoints_[index]; }

SrvSelectCharReply& SrvSelectCharReply::set_factionPoints(const uint16_t* data) { for (size_t index = 0; index < 10; ++index) factionPoints_[index] = data[index]; return *this; }

SrvSelectCharReply& SrvSelectCharReply::set_factionPoints(const uint16_t& data, size_t index) { factionPoints_[index] = data; return *this; }

uint32_t SrvSelectCharReply::guildId() const { return guildId_; }

SrvSelectCharReply& SrvSelectCharReply::set_guildId(uint32_t data) { guildId_ = data; return *this; }

uint16_t SrvSelectCharReply::guildContribution() const { return guildContribution_; }

SrvSelectCharReply& SrvSelectCharReply::set_guildContribution(uint16_t data) { guildContribution_ = data; return *this; }

uint8_t SrvSelectCharReply::guildRank() const { return guildRank_; }

SrvSelectCharReply& SrvSelectCharReply::set_guildRank(uint8_t data) { guildRank_ = data; return *this; }

uint16_t SrvSelectCharReply::pkFlag() const { return pkFlag_; }

SrvSelectCharReply& SrvSelectCharReply::set_pkFlag(uint16_t data) { pkFlag_ = data; return *this; }

uint16_t SrvSelectCharReply::stamina() const { return stamina_; }

SrvSelectCharReply& SrvSelectCharReply::set_stamina(uint16_t data) { stamina_ = data; return *this; }

const StatusEffect* SrvSelectCharReply::effects() const { return effects_; }

const StatusEffect& SrvSelectCharReply::effects(size_t index) const { return effects_[index]; }

SrvSelectCharReply& SrvSelectCharReply::set_effects(const StatusEffect* data) { for (size_t index = 0; index < StatusEffects::maxEffects; ++index) effects_[index] = data[index]; return *this; }

SrvSelectCharReply& SrvSelectCharReply::set_effects(const StatusEffect& data, size_t index) { effects_[index] = data; return *this; }

uint16_t SrvSelectCharReply::patHp() const { return patHp_; }

SrvSelectCharReply& SrvSelectCharReply::set_patHp(uint16_t data) { patHp_ = data; return *this; }

uint32_t SrvSelectCharReply::patCooldownTime() const { return patCooldownTime_; }

SrvSelectCharReply& SrvSelectCharReply::set_patCooldownTime(uint32_t data) { patCooldownTime_ = data; return *this; }

const uint16_t* SrvSelectCharReply::skills() const { return skills_; }

const uint16_t& SrvSelectCharReply::skills(size_t index) const { return skills_[index]; }

SrvSelectCharReply& SrvSelectCharReply::set_skills(const uint16_t* data) { for (size_t index = 0; index < Skills::maxSkills; ++index) skills_[index] = data[index]; return *this; }

SrvSelectCharReply& SrvSelectCharReply::set_skills(const uint16_t& data, size_t index) { skills_[index] = data; return *this; }

const uint16_t* SrvSelectCharReply::hotbar() const { return hotbar_; }

const uint16_t& SrvSelectCharReply::hotbar(size_t index) const { return hotbar_[index]; }

SrvSelectCharReply& SrvSelectCharReply::set_hotbar(const uint16_t* data) { for (size_t index = 0; index < Hotbar::maxItems; ++index) hotbar_[index] = data[index]; return *this; }

SrvSelectCharReply& SrvSelectCharReply::set_hotbar(const uint16_t& data, size_t index) { hotbar_[index] = data; return *this; }

uint32_t SrvSelectCharReply::tag() const { return tag_; }

SrvSelectCharReply& SrvSelectCharReply::set_tag(uint32_t data) { tag_ = data; return *this; }

const std::string& SrvSelectCharReply::name() const { return name_; }

SrvSelectCharReply& SrvSelectCharReply::set_name(const std::string& data) { name_ = data; return *this; }


void SrvSelectCharReply::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(race_);
	writer.set_uint16_t(map_);
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_uint16_t(spawn_);
	writer.set_uint32_t(mask_);
	writer.set_uint32_t(headGear_);
	for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) writer.set_uint32_t(inventory_[index]);
	writer.set_uint8_t(stone_);
	writer.set_uint8_t(face_);
	writer.set_uint8_t(hair_);
	writer.set_uint16_t(job_);
	writer.set_uint8_t(factionId_);
	writer.set_uint8_t(factionRank_);
	writer.set_uint8_t(fame_);
	writer.set_uint16_t(str_);
	writer.set_uint16_t(dex_);
	writer.set_uint16_t(intel_);
	writer.set_uint16_t(con_);
	writer.set_uint16_t(charm_);
	writer.set_uint16_t(sense_);
	writer.set_int32_t(hp_);
	writer.set_int32_t(mp_);
	writer.set_uint32_t(xp_);
	writer.set_uint16_t(level_);
	writer.set_uint32_t(statPoints_);
	writer.set_uint32_t(skillPoints_);
	writer.set_uint8_t(bodySize_);
	writer.set_uint8_t(headSize_);
	writer.set_uint32_t(penaltyXp_);
	for (size_t index = 0; index < 2; ++index) writer.set_uint16_t(factionFame_[index]);
	for (size_t index = 0; index < 10; ++index) writer.set_uint16_t(factionPoints_[index]);
	writer.set_uint32_t(guildId_);
	writer.set_uint16_t(guildContribution_);
	writer.set_uint8_t(guildRank_);
	writer.set_uint16_t(pkFlag_);
	writer.set_uint16_t(stamina_);
	for (size_t index = 0; index < StatusEffects::maxEffects; ++index) writer.set_iserialize(effects_[index]);
	writer.set_uint16_t(patHp_);
	writer.set_uint32_t(patCooldownTime_);
	for (size_t index = 0; index < Skills::maxSkills; ++index) writer.set_uint16_t(skills_[index]);
	for (size_t index = 0; index < Hotbar::maxItems; ++index) writer.set_uint16_t(hotbar_[index]);
	writer.set_uint32_t(tag_);
	writer.set_string(name_);
}

uint16_t SrvSelectCharReply::get_size() const {
	uint16_t size = 0;
	size += sizeof(race_);
	size += sizeof(map_);
	size += sizeof(x_);
	size += sizeof(y_);
	size += sizeof(spawn_);
	size += sizeof(mask_);
	size += sizeof(headGear_);
	size += sizeof(inventory_);
	size += sizeof(stone_);
	size += sizeof(face_);
	size += sizeof(hair_);
	size += sizeof(job_);
	size += sizeof(factionId_);
	size += sizeof(factionRank_);
	size += sizeof(fame_);
	size += sizeof(str_);
	size += sizeof(dex_);
	size += sizeof(intel_);
	size += sizeof(con_);
	size += sizeof(charm_);
	size += sizeof(sense_);
	size += sizeof(hp_);
	size += sizeof(mp_);
	size += sizeof(xp_);
	size += sizeof(level_);
	size += sizeof(statPoints_);
	size += sizeof(skillPoints_);
	size += sizeof(bodySize_);
	size += sizeof(headSize_);
	size += sizeof(penaltyXp_);
	size += sizeof(factionFame_);
	size += sizeof(factionPoints_);
	size += sizeof(guildId_);
	size += sizeof(guildContribution_);
	size += sizeof(guildRank_);
	size += sizeof(pkFlag_);
	size += sizeof(stamina_);
	size += sizeof(effects_);
	size += sizeof(patHp_);
	size += sizeof(patCooldownTime_);
	size += sizeof(skills_);
	size += sizeof(hotbar_);
	size += sizeof(tag_);
	size += sizeof(char) * (name_.size() + 1);
	return size;
}


SrvSelectCharReply SrvSelectCharReply::create(Entity entity) {
	const auto entity_advancedinfo = entity.component<AdvancedInfo>();
	const auto entity_statuseffects = entity.component<StatusEffects>();
	const auto entity_characterinfo = entity.component<CharacterInfo>();
	const auto entity_skills = entity.component<Skills>();
	const auto entity_position = entity.component<Position>();
	const auto entity_charactergraphics = entity.component<CharacterGraphics>();
	const auto entity_basicinfo = entity.component<BasicInfo>();
	const auto entity_inventory = entity.component<Inventory>();
	const auto entity_hotbar = entity.component<Hotbar>();
	const auto entity_stats = entity.component<Stats>();
	uint32_t inventory_[Inventory::maxVisibleEquippedItems];
	for (size_t index = 0; index < Inventory::maxVisibleEquippedItems; ++index) inventory_[index] = entity_inventory->getEquipped()[index].getVisible();
	uint16_t factionFame_[2];
	for (size_t index = 0; index < 2; ++index) factionFame_[index] = entity_characterinfo->factionFame_[index];
	uint16_t factionPoints_[10];
	for (size_t index = 0; index < 10; ++index) factionPoints_[index] = entity_characterinfo->factionPoints_[index];
	StatusEffect effects_[StatusEffects::maxEffects];
	for (size_t index = 0; index < StatusEffects::maxEffects; ++index) effects_[index] = entity_statuseffects->effects_[index];
	uint16_t skills_[Skills::maxSkills];
	for (size_t index = 0; index < Skills::maxSkills; ++index) skills_[index] = entity_skills->skills_[index].id_;
	uint16_t hotbar_[Hotbar::maxItems];
	for (size_t index = 0; index < Hotbar::maxItems; ++index) hotbar_[index] = entity_hotbar->items_[index].item_;

	return SrvSelectCharReply(entity_charactergraphics->race_, entity_position->map_, entity_position->x_, entity_position->y_, entity_position->spawn_, entity_charactergraphics->face_, entity_charactergraphics->hair_, inventory_, entity_characterinfo->stone_, entity_charactergraphics->face_, entity_charactergraphics->hair_, entity_characterinfo->job_, entity_characterinfo->factionId_, entity_characterinfo->factionRank_, entity_characterinfo->fame_, entity_stats->str_, entity_stats->dex_, entity_stats->int_, entity_stats->con_, entity_stats->charm_, entity_stats->sense_, entity_advancedinfo->hp_, entity_advancedinfo->mp_, entity_basicinfo->xp_, entity_basicinfo->level_, entity_characterinfo->statPoints_, entity_characterinfo->skillPoints_, entity_stats->bodySize_, entity_stats->headSize_, entity_characterinfo->penaltyXp_, factionFame_, factionPoints_, entity_characterinfo->guildId_, entity_characterinfo->guildContribution_, entity_characterinfo->guildRank_, entity_characterinfo->pkFlag_, entity_characterinfo->stamina_, effects_, entity_characterinfo->patHp_, entity_characterinfo->patCooldownTime_.count(), skills_, hotbar_, entity_basicinfo->tag_, entity_basicinfo->name_);
}

SrvSelectCharReply SrvSelectCharReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvSelectCharReply(reader);
}

}