#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "iserialize.h"
#include <string>
#include <array>
#include <memory>
#include "item.h"

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
		uint16_t &zone() {return zone_; }
		std::string &name() {return name_; }
		uint8_t &charId() { return charId_; }
		uint8_t &runMode() { return runMode_; }
		uint8_t &rideMode() { return rideMode_; }
		uint8_t &weightRate() { return weightRate_; }
		uint16_t &positionZ() { return positionZ_; }
		uint32_t &secUntilDel() { return secUntilDel_; }
		uint16_t &level() { return level_; }
		uint16_t &job() { return job_; }
		uint8_t &platinium() { return platinium_; }

		template <ItemPosition itemPos>
		const Item &item() const { return *equipedItems_[itemPos]; }

		template <ItemPosition itemPos>
		void item(std::unique_ptr<Item> i) { equipedItems_[itemPos] = std::move(i); }


	protected:
		virtual void serialize(CRosePacket &os) const {}
		virtual void deserialize(CRosePacket &os) {}

	private:
		uint8_t							race_;
		uint8_t							stone_;
		uint8_t							hair_;
		uint8_t							face_;
		uint16_t						zone_; // map id
		std::string						name_;
		uint8_t							charId_;
		uint8_t							runMode_;
		uint8_t							rideMode_;
		uint8_t							weightRate_; // current amount of weight / max amount * 100
		uint16_t						positionZ_;
		uint32_t						secUntilDel_;
		uint16_t						level_;
		uint16_t						job_;
		uint8_t							platinium_;
		std::array<std::unique_ptr<Item>, MAX_EQUIPPED_ITEMS>	equipedItems_;
};

#endif
