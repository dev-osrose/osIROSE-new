#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_MOB_CHAR, SrvMobChar)
REGISTER_SEND_PACKET(ePacketType::PAKWC_MOB_CHAR, SrvMobChar)
class SrvMobChar : public CRosePacket {
	public:
		SrvMobChar();
		SrvMobChar(CRoseReader reader);
	private:
		SrvMobChar(uint16_t id, float x, float y, float destX, float destY, uint16_t command, uint16_t targetId, uint8_t moveMode, int32_t hp, int32_t teamId, uint32_t statusFlag, uint16_t npcId, uint16_t questId);
	public:

		virtual ~SrvMobChar() = default;

		uint16_t id() const;
		SrvMobChar& set_id(uint16_t);
		float x() const;
		SrvMobChar& set_x(float);
		float y() const;
		SrvMobChar& set_y(float);
		float destX() const;
		SrvMobChar& set_destX(float);
		float destY() const;
		SrvMobChar& set_destY(float);
		uint16_t command() const;
		SrvMobChar& set_command(uint16_t);
		uint16_t targetId() const;
		SrvMobChar& set_targetId(uint16_t);
		uint8_t moveMode() const;
		SrvMobChar& set_moveMode(uint8_t);
		int32_t hp() const;
		SrvMobChar& set_hp(int32_t);
		int32_t teamId() const;
		SrvMobChar& set_teamId(int32_t);
		uint32_t statusFlag() const;
		SrvMobChar& set_statusFlag(uint32_t);
		uint16_t npcId() const;
		SrvMobChar& set_npcId(uint16_t);
		uint16_t questId() const;
		SrvMobChar& set_questId(uint16_t);

		static SrvMobChar create(Entity entity);
		static SrvMobChar create(uint8_t *buffer);

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
		uint16_t npcId_;
		uint16_t questId_;
};

}