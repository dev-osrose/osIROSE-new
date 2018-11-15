#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_SET_HP_AND_MP, SrvSetHpAndMp)
REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_HP_AND_MP, SrvSetHpAndMp)
class SrvSetHpAndMp : public CRosePacket {
	public:
		SrvSetHpAndMp();
		SrvSetHpAndMp(CRoseReader reader);
	private:
		SrvSetHpAndMp(uint16_t id, uint16_t hp, uint16_t mp);
	public:

		virtual ~SrvSetHpAndMp() = default;

		uint16_t id() const;
		SrvSetHpAndMp& set_id(uint16_t);
		uint16_t hp() const;
		SrvSetHpAndMp& set_hp(uint16_t);
		uint16_t mp() const;
		SrvSetHpAndMp& set_mp(uint16_t);

		static SrvSetHpAndMp create(Entity entity, uint16_t hp, uint16_t mp);
		static SrvSetHpAndMp create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t id_;
		uint16_t hp_;
		uint16_t mp_;
};

}