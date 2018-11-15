#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_SELECT_CHAR_REPLY, SrvSelectCharReply)
REGISTER_SEND_PACKET(ePacketType::PAKWC_SELECT_CHAR_REPLY, SrvSelectCharReply)
class SrvSelectCharReply : public CRosePacket {
	public:
		SrvSelectCharReply();
		SrvSelectCharReply(CRoseReader reader);
	private:
		SrvSelectCharReply(uint8_t race, uint16_t map, float x, float y, uint16_t spawn, uint32_t mask, uint32_t headGear, uint32_t inventory[Inventory::maxVisibleEquippedItems], uint8_t stone, uint8_t face, uint8_t hair, uint16_t job, uint8_t factionId, uint8_t factionRank, uint8_t fame, uint16_t str, uint16_t dex, uint16_t intel, uint16_t con, uint16_t charm, uint16_t sense, int32_t hp, int32_t mp, uint32_t xp, uint16_t level, uint32_t statPoints, uint32_t skillPoints, uint8_t bodySize, uint8_t headSize, uint32_t penaltyXp, uint16_t factionFame[2], uint16_t factionPoints[10], uint32_t guildId, uint16_t guildContribution, uint8_t guildRank, uint16_t pkFlag, uint16_t stamina, StatusEffect effects[StatusEffects::maxEffects], uint16_t patHp, uint32_t patCooldownTime, uint16_t skills[Skills::maxSkills], uint16_t hotbar[Hotbar::maxItems], uint32_t tag, const std::string& name);
	public:

		virtual ~SrvSelectCharReply() = default;

		uint8_t race() const;
		SrvSelectCharReply& set_race(uint8_t);
		uint16_t map() const;
		SrvSelectCharReply& set_map(uint16_t);
		float x() const;
		SrvSelectCharReply& set_x(float);
		float y() const;
		SrvSelectCharReply& set_y(float);
		uint16_t spawn() const;
		SrvSelectCharReply& set_spawn(uint16_t);
		uint32_t mask() const;
		SrvSelectCharReply& set_mask(uint32_t);
		uint32_t headGear() const;
		SrvSelectCharReply& set_headGear(uint32_t);
		const uint32_t* inventory() const;
		const uint32_t& inventory(size_t index) const;
		SrvSelectCharReply& set_inventory(const uint32_t[Inventory::maxVisibleEquippedItems]);
		SrvSelectCharReply& set_inventory(const uint32_t&, size_t index);
		uint8_t stone() const;
		SrvSelectCharReply& set_stone(uint8_t);
		uint8_t face() const;
		SrvSelectCharReply& set_face(uint8_t);
		uint8_t hair() const;
		SrvSelectCharReply& set_hair(uint8_t);
		uint16_t job() const;
		SrvSelectCharReply& set_job(uint16_t);
		uint8_t factionId() const;
		SrvSelectCharReply& set_factionId(uint8_t);
		uint8_t factionRank() const;
		SrvSelectCharReply& set_factionRank(uint8_t);
		uint8_t fame() const;
		SrvSelectCharReply& set_fame(uint8_t);
		uint16_t str() const;
		SrvSelectCharReply& set_str(uint16_t);
		uint16_t dex() const;
		SrvSelectCharReply& set_dex(uint16_t);
		uint16_t intel() const;
		SrvSelectCharReply& set_intel(uint16_t);
		uint16_t con() const;
		SrvSelectCharReply& set_con(uint16_t);
		uint16_t charm() const;
		SrvSelectCharReply& set_charm(uint16_t);
		uint16_t sense() const;
		SrvSelectCharReply& set_sense(uint16_t);
		int32_t hp() const;
		SrvSelectCharReply& set_hp(int32_t);
		int32_t mp() const;
		SrvSelectCharReply& set_mp(int32_t);
		uint32_t xp() const;
		SrvSelectCharReply& set_xp(uint32_t);
		uint16_t level() const;
		SrvSelectCharReply& set_level(uint16_t);
		uint32_t statPoints() const;
		SrvSelectCharReply& set_statPoints(uint32_t);
		uint32_t skillPoints() const;
		SrvSelectCharReply& set_skillPoints(uint32_t);
		uint8_t bodySize() const;
		SrvSelectCharReply& set_bodySize(uint8_t);
		uint8_t headSize() const;
		SrvSelectCharReply& set_headSize(uint8_t);
		uint32_t penaltyXp() const;
		SrvSelectCharReply& set_penaltyXp(uint32_t);
		const uint16_t* factionFame() const;
		const uint16_t& factionFame(size_t index) const;
		SrvSelectCharReply& set_factionFame(const uint16_t[2]);
		SrvSelectCharReply& set_factionFame(const uint16_t&, size_t index);
		const uint16_t* factionPoints() const;
		const uint16_t& factionPoints(size_t index) const;
		SrvSelectCharReply& set_factionPoints(const uint16_t[10]);
		SrvSelectCharReply& set_factionPoints(const uint16_t&, size_t index);
		uint32_t guildId() const;
		SrvSelectCharReply& set_guildId(uint32_t);
		uint16_t guildContribution() const;
		SrvSelectCharReply& set_guildContribution(uint16_t);
		uint8_t guildRank() const;
		SrvSelectCharReply& set_guildRank(uint8_t);
		uint16_t pkFlag() const;
		SrvSelectCharReply& set_pkFlag(uint16_t);
		uint16_t stamina() const;
		SrvSelectCharReply& set_stamina(uint16_t);
		const StatusEffect* effects() const;
		const StatusEffect& effects(size_t index) const;
		SrvSelectCharReply& set_effects(const StatusEffect[StatusEffects::maxEffects]);
		SrvSelectCharReply& set_effects(const StatusEffect&, size_t index);
		uint16_t patHp() const;
		SrvSelectCharReply& set_patHp(uint16_t);
		uint32_t patCooldownTime() const;
		SrvSelectCharReply& set_patCooldownTime(uint32_t);
		const uint16_t* skills() const;
		const uint16_t& skills(size_t index) const;
		SrvSelectCharReply& set_skills(const uint16_t[Skills::maxSkills]);
		SrvSelectCharReply& set_skills(const uint16_t&, size_t index);
		const uint16_t* hotbar() const;
		const uint16_t& hotbar(size_t index) const;
		SrvSelectCharReply& set_hotbar(const uint16_t[Hotbar::maxItems]);
		SrvSelectCharReply& set_hotbar(const uint16_t&, size_t index);
		uint32_t tag() const;
		SrvSelectCharReply& set_tag(uint32_t);
		const std::string& name() const;
		SrvSelectCharReply& set_name(const std::string&);

		static SrvSelectCharReply create(Entity entity);
		static SrvSelectCharReply create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint8_t race_;
		uint16_t map_;
		float x_;
		float y_;
		uint16_t spawn_;
		uint32_t mask_;
		uint32_t headGear_;
		uint32_t inventory_[Inventory::maxVisibleEquippedItems];
		uint8_t stone_;
		uint8_t face_;
		uint8_t hair_;
		uint16_t job_;
		uint8_t factionId_;
		uint8_t factionRank_;
		uint8_t fame_;
		uint16_t str_;
		uint16_t dex_;
		uint16_t intel_;
		uint16_t con_;
		uint16_t charm_;
		uint16_t sense_;
		int32_t hp_;
		int32_t mp_;
		uint32_t xp_;
		uint16_t level_;
		uint32_t statPoints_;
		uint32_t skillPoints_;
		uint8_t bodySize_;
		uint8_t headSize_;
		uint32_t penaltyXp_;
		uint16_t factionFame_[2];
		uint16_t factionPoints_[10];
		uint32_t guildId_;
		uint16_t guildContribution_;
		uint8_t guildRank_;
		uint16_t pkFlag_;
		uint16_t stamina_;
		StatusEffect effects_[StatusEffects::maxEffects];
		uint16_t patHp_;
		uint32_t patCooldownTime_;
		uint16_t skills_[Skills::maxSkills];
		uint16_t hotbar_[Hotbar::maxItems];
		uint32_t tag_;
		std::string name_;
};

}