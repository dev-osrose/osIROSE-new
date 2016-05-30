#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "iserialize.h"
#include <string>
#include <array>
#include <memory>
#include "item.h"

class Position {
	public:
		Position() : mapId(-1) {}
		Position(int mapId, int x, int y, int z) : mapId_(mapId), x_(x), y_(y), z_(z) {}

		int maxId_;
		int x_;
		int y_;
		int z_;
};

class StatusEffect {
	public:
		StatusEffect(uint32_t secondsLeft, uint16_t value, uint16_t unkown) : secondsLeft_(secondsLeft), value_(value), unknown_(unknown) {}

		uint32_t secondsLeft_;
		uint16_t value_;
		uint16_t unknown_;
};

class Character : public ISerialize {
	public:
		enum ItemPosition {
		   EQUIP_FACE = 0,
		   EQUIP_HAIR = 1,
		   EQUIP_HELMET = 2,
		   EQUIP_ARMOR = 3,
		   EQUIP_GAUNTLET = 4,
		   EQUIP_BOOTS = 5,
		   EQUIP_GOGGLES = 6,
		   EQUIP_FACE_ITEM = EQUIP_GOGGLES,
		   EQUIP_BACKPACK = 7,
		   EQUIP_WEAPON_R = 8,
		   EQUIP_WEAPON_L = 9,
		   MAX_EQUIPPED_ITEMS
		};

		CCharacter() {}
		virtual ~CCharacter() {}

		uint8_t &race() { return race_; }
		uint8_t &stone() { return stone_; }
		uint8_t &hair() { return hair_; if (!equipedItems_[EQUIP_HAIR]) equipedItems_[EQUIP_HAIR] = new Item(hair, 0, 0, 0); }
		uint8_t &face() { return face_; if (!equipedItems_[EQUIP_FACE]) equipedITems_[EQUIP_FACE] = new Item(face, 0, 0, 0); }
		std::string &name() {return name_; }
		uint8_t &charId() { return charId_; }
		uint8_t &runMode() { return runMode_; }
		uint8_t &rideMode() { return rideMode_; }
		uint8_t &weightRate() { return weightRate_; }
		uint32_t &secUntilDel() { return secUntilDel_; }
		uint16_t &level() { return level_; }
		uint16_t &job() { return job_; }
		uint8_t &platinium() { return platinium_; }
		uint16_t &currentHp() { return currentHp_; }
		uint16_t &currentMp() { return currentMp_; }
		uint16_t &maxHp() { return maxHp_; }
		uint16_t &maxMp() { return maxMp_; }
		uint64_t &penalizeExp() { return penqlizeExp_; }
		uint64_t &currentExp() { return currentExp_; }
		uint32_t &teamNumber() { return teamNumber_; }
		uint32_t &worldTime() { return worldTime_; }

		Position &currentPos() { return currentPos_; }
		Position &savedPos() { return savedPos_; }
		Position &revivePos() { return revivePos_; }
		Position &startPos() { return startPos_; }

		template <ItemPosition itemPos>
		const Item &equippedItem() const { return *equipedItems_[itemPos]; }

		template <ItemPosition itemPos>
		void equippedItem(std::unique_ptr<Item> i) { equipedItems_[itemPos] = std::move(i); }

	protected:
		virtual void serialize(CRosePacket &os) const {}
		virtual void deserialize(CRosePacket &os) {}

	private:
		uint16_t						currentHp_;
		uint16_t						maxHp_;
		uint16_t						currentMp_;
		uint16_t						maxMp_;
		uint64_t						penalizeExp_;
		uint64_t						currentExp_;
		uint32_t						teamNumber_;
		uint32_t						worldTime_;
		uint8_t							race_;
		uint8_t							stone_;
		uint8_t							hair_;
		uint8_t							face_;
		std::string						name_;
		uint8_t							charId_;
		uint8_t							runMode_;
		uint8_t							rideMode_;
		uint8_t							weightRate_; // current amount of weight / max amount * 100
		uint32_t						secUntilDel_;
		uint16_t						level_;
		uint16_t						job_;
		uint8_t							platinium_;
		uint8_t							union_;
		uint8_t							rank_;
		uint8_t							fame_;
		uint16_t						str_;
		uint16_t						dex_;
		uint16_t						int_;
		uint16_t						con_;
		uint16_t						charm_;
		uint16_t						sense_;
		uint16_t						statPoints_;
		uint16_t						skillPoints_;
		uint8_t							bodySize_;
		uint8_t							headSize_;
		uint16_t						pkFlag_;
		uint16_t						stamina_;
		uint32_t						guildId_;
		uint16_t						guildContribution_;
		uint8_t							guildPosition_;
		uint16_t						patHp_;
		uint32_t						patCooldownTime_;
		uint64_t						zuly_;

		Position						currentPos_;
		Position						savedPos_;
		Position						revivePos_;
		Position						startPos_;

		std::vector<StatusEffect>		statusEffects_;

		std::array<std::unique_ptr<Item>, MAX_EQUIPPED_ITEMS>	equipedItems_;
		std::array<std::unique_ptr<Item>, 1>	inventory_;
};

#endif
