#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_ATTACK, SrvAttack)
REGISTER_SEND_PACKET(ePacketType::PAKWC_ATTACK, SrvAttack)
class SrvAttack : public CRosePacket {
	public:
		SrvAttack();
		SrvAttack(CRoseReader reader);
	private:
		SrvAttack(uint16_t attackerId, uint16_t defenderId, uint16_t dist, float x, float y, uint16_t z);
	public:

		virtual ~SrvAttack() = default;

		uint16_t attackerId() const;
		SrvAttack& set_attackerId(uint16_t);
		uint16_t defenderId() const;
		SrvAttack& set_defenderId(uint16_t);
		uint16_t dist() const;
		SrvAttack& set_dist(uint16_t);
		float x() const;
		SrvAttack& set_x(float);
		float y() const;
		SrvAttack& set_y(float);
		uint16_t z() const;
		SrvAttack& set_z(uint16_t);

		static SrvAttack create(Entity entityA, Entity entityD);
		static SrvAttack create(uint8_t *buffer);
		static std::unique_ptr<SrvAttack> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t attackerId_;
		uint16_t defenderId_;
		uint16_t dist_;
		float x_;
		float y_;
		uint16_t z_;
};

}