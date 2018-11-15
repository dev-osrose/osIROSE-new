#include "srv_billingmessage.h"
#include "throwassert.h"

namespace RoseCommon {

SrvBillingMessage::SrvBillingMessage() : CRosePacket(ePacketType::PAKWC_BILLING_MESSAGE) {}

SrvBillingMessage::SrvBillingMessage(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_BILLING_MESSAGE, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(functionType_);
	reader.get_uint32_t(payFlag_);
}

uint16_t SrvBillingMessage::functionType() const { return functionType_; }

SrvBillingMessage& SrvBillingMessage::set_functionType(uint16_t data) { functionType_ = data; return *this; }

uint32_t SrvBillingMessage::payFlag() const { return payFlag_; }

SrvBillingMessage& SrvBillingMessage::set_payFlag(uint32_t data) { payFlag_ = data; return *this; }


void SrvBillingMessage::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(functionType_);
	writer.set_uint32_t(payFlag_);
}

uint16_t SrvBillingMessage::get_size() const {
	uint16_t size = 0;
	size += sizeof(functionType_);
	size += sizeof(payFlag_);
	return size;
}


SrvBillingMessage SrvBillingMessage::create() {


	return SrvBillingMessage();
}

SrvBillingMessage SrvBillingMessage::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvBillingMessage(reader);
}

}