#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_HP_REPLY, SrvHpReply)
REGISTER_SEND_PACKET(ePacketType::PAKWC_HP_REPLY, SrvHpReply)
class SrvHpReply : public CRosePacket {
	public:
		SrvHpReply();
		SrvHpReply(CRoseReader reader);
	private:
		SrvHpReply(uint16_t id, int32_t hp);
	public:

		virtual ~SrvHpReply() = default;

		uint16_t id() const;
		SrvHpReply& set_id(uint16_t);
		int32_t hp() const;
		SrvHpReply& set_hp(int32_t);

		static SrvHpReply create(Entity entity);
		static SrvHpReply create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t id_;
		int32_t hp_;
};

}