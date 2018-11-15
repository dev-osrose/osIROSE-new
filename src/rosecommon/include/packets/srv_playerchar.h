#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_PLAYER_CHAR, SrvPlayerChar)
REGISTER_SEND_PACKET(ePacketType::PAKWC_PLAYER_CHAR, SrvPlayerChar)
class SrvPlayerChar : public CRosePacket {
	public:
		SrvPlayerChar();
		SrvPlayerChar(CRoseReader reader);
	private:
		SrvPlayerChar(uint16_t id, float x, float y, float destX, float destY, uint16_t command, uint16_t targetId, uint8_t moveMode, int32_t hp, int32_t teamId, uint32_t statusFlag, uint8_t race, uint16_t runSpeed, uint16_t atkSpeed, uint8_t weightRate, uint32_t face, uint32_t hair, uint32_t inventory[Inventory::maxVisibleEquippedItems], uint16_t bullets[BulletItems::BulletType::MAX_BULLET_TYPES], uint16_t job, uint8_t level, uint32_t ridingItems[RidingItems::RidingPosition::MAX_RIDING_ITEMS], uint16_t z, uint32_t subFlag, const std::string& name, const std::string& fakeName);
	public:

		virtual ~SrvPlayerChar() = default;

		uint16_t id() const;
		SrvPlayerChar& set_id(uint16_t);
		float x() const;
		SrvPlayerChar& set_x(float);
		float y() const;
		SrvPlayerChar& set_y(float);
		float destX() const;
		SrvPlayerChar& set_destX(float);
		float destY() const;
		SrvPlayerChar& set_destY(float);
		uint16_t command() const;
		SrvPlayerChar& set_command(uint16_t);
		uint16_t targetId() const;
		SrvPlayerChar& set_targetId(uint16_t);
		uint8_t moveMode() const;
		SrvPlayerChar& set_moveMode(uint8_t);
		int32_t hp() const;
		SrvPlayerChar& set_hp(int32_t);
		int32_t teamId() const;
		SrvPlayerChar& set_teamId(int32_t);
		uint32_t statusFlag() const;
		SrvPlayerChar& set_statusFlag(uint32_t);
		uint8_t race() const;
		SrvPlayerChar& set_race(uint8_t);
		uint16_t runSpeed() const;
		SrvPlayerChar& set_runSpeed(uint16_t);
		uint16_t atkSpeed() const;
		SrvPlayerChar& set_atkSpeed(uint16_t);
		uint8_t weightRate() const;
		SrvPlayerChar& set_weightRate(uint8_t);
		uint32_t face() const;
		SrvPlayerChar& set_face(uint32_t);
		uint32_t hair() const;
		SrvPlayerChar& set_hair(uint32_t);
		const uint32_t* inventory() const;
		const uint32_t& inventory(size_t index) const;
		SrvPlayerChar& set_inventory(const uint32_t[Inventory::maxVisibleEquippedItems]);
		SrvPlayerChar& set_inventory(const uint32_t&, size_t index);
		const uint16_t* bullets() const;
		const uint16_t& bullets(size_t index) const;
		SrvPlayerChar& set_bullets(const uint16_t[BulletItems::BulletType::MAX_BULLET_TYPES]);
		SrvPlayerChar& set_bullets(const uint16_t&, size_t index);
		uint16_t job() const;
		SrvPlayerChar& set_job(uint16_t);
		uint8_t level() const;
		SrvPlayerChar& set_level(uint8_t);
		const uint32_t* ridingItems() const;
		const uint32_t& ridingItems(size_t index) const;
		SrvPlayerChar& set_ridingItems(const uint32_t[RidingItems::RidingPosition::MAX_RIDING_ITEMS]);
		SrvPlayerChar& set_ridingItems(const uint32_t&, size_t index);
		uint16_t z() const;
		SrvPlayerChar& set_z(uint16_t);
		uint32_t subFlag() const;
		SrvPlayerChar& set_subFlag(uint32_t);
		const std::string& name() const;
		SrvPlayerChar& set_name(const std::string&);
		const std::string& fakeName() const;
		SrvPlayerChar& set_fakeName(const std::string&);

		static SrvPlayerChar create(Entity entity);
		static SrvPlayerChar create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t id_;
		float x_;
		float y_;
		float destX_;
		float destY_;
		uint16_t command_;
		uint16_t targetId_;
		uint8_t moveMode_;
		int32_t hp_;
		int32_t teamId_;
		uint32_t statusFlag_;
		uint8_t race_;
		uint16_t runSpeed_;
		uint16_t atkSpeed_;
		uint8_t weightRate_;
		uint32_t face_;
		uint32_t hair_;
		uint32_t inventory_[Inventory::maxVisibleEquippedItems];
		uint16_t bullets_[BulletItems::BulletType::MAX_BULLET_TYPES];
		uint16_t job_;
		uint8_t level_;
		uint32_t ridingItems_[RidingItems::RidingPosition::MAX_RIDING_ITEMS];
		uint16_t z_;
		uint32_t subFlag_;
		std::string name_;
		std::string fakeName_;
};

}