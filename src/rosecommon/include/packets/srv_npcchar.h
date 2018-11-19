#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_NPC_CHAR, SrvNpcChar)
REGISTER_SEND_PACKET(ePacketType::PAKWC_NPC_CHAR, SrvNpcChar)
class SrvNpcChar : public CRosePacket {
	public:
		SrvNpcChar();
		SrvNpcChar(CRoseReader reader);
	private:
		SrvNpcChar(uint16_t id, float x, float y, float destX, float destY, uint16_t command, uint16_t targetId, uint8_t moveMode, int32_t hp, int32_t teamId, uint32_t statusFlag, uint16_t npcId, uint16_t questId, float angle);
	public:

		virtual ~SrvNpcChar() = default;

		uint16_t id() const;
		SrvNpcChar& set_id(uint16_t);
		float x() const;
		SrvNpcChar& set_x(float);
		float y() const;
		SrvNpcChar& set_y(float);
		float destX() const;
		SrvNpcChar& set_destX(float);
		float destY() const;
		SrvNpcChar& set_destY(float);
		uint16_t command() const;
		SrvNpcChar& set_command(uint16_t);
		uint16_t targetId() const;
		SrvNpcChar& set_targetId(uint16_t);
		uint8_t moveMode() const;
		SrvNpcChar& set_moveMode(uint8_t);
		int32_t hp() const;
		SrvNpcChar& set_hp(int32_t);
		int32_t teamId() const;
		SrvNpcChar& set_teamId(int32_t);
		uint32_t statusFlag() const;
		SrvNpcChar& set_statusFlag(uint32_t);
		uint16_t npcId() const;
		SrvNpcChar& set_npcId(uint16_t);
		uint16_t questId() const;
		SrvNpcChar& set_questId(uint16_t);
		float angle() const;
		SrvNpcChar& set_angle(float);
		uint16_t eventStatus() const;
		SrvNpcChar& set_eventStatus(uint16_t);

		static SrvNpcChar create(Entity entity);
		static SrvNpcChar create(uint8_t *buffer);
		static std::unique_ptr<SrvNpcChar> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

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
		float angle_;
		uint16_t eventStatus_= 0;
};

}