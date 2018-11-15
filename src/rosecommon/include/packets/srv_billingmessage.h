#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_BILLING_MESSAGE, SrvBillingMessage)
REGISTER_SEND_PACKET(ePacketType::PAKWC_BILLING_MESSAGE, SrvBillingMessage)
class SrvBillingMessage : public CRosePacket {
	public:
		SrvBillingMessage();
		SrvBillingMessage(CRoseReader reader);
	public:

		virtual ~SrvBillingMessage() = default;

		uint16_t functionType() const;
		SrvBillingMessage& set_functionType(uint16_t);
		uint32_t payFlag() const;
		SrvBillingMessage& set_payFlag(uint32_t);

		static SrvBillingMessage create();
		static SrvBillingMessage create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t functionType_= 0x1001;
		uint32_t payFlag_= 2;
};

}