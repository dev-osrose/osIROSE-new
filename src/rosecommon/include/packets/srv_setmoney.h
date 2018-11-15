#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_SET_MONEY, SrvSetMoney)
REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_MONEY, SrvSetMoney)
class SrvSetMoney : public CRosePacket {
	public:
		SrvSetMoney();
		SrvSetMoney(CRoseReader reader);
	private:
		SrvSetMoney(int64_t zuly);
	public:

		virtual ~SrvSetMoney() = default;

		int64_t zuly() const;
		SrvSetMoney& set_zuly(int64_t);

		static SrvSetMoney create(Entity entity);
		static SrvSetMoney create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		int64_t zuly_;
};

}