#include "srv_billing_message.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvBillingMessage::SrvBillingMessage() : CRosePacket(SrvBillingMessage::PACKET_ID) {}

SrvBillingMessage::SrvBillingMessage(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(functionType)) {
        return;
    }
    if (!reader.get_uint32_t(payFlag)) {
        return;
    }
}

void SrvBillingMessage::set_functionType(const uint16_t functionType) {
    this->functionType = functionType;
}

uint16_t SrvBillingMessage::get_functionType() const {
    return functionType;
}

void SrvBillingMessage::set_payFlag(const uint32_t payFlag) {
    this->payFlag = payFlag;
}

uint32_t SrvBillingMessage::get_payFlag() const {
    return payFlag;
}

SrvBillingMessage SrvBillingMessage::create() {
    SrvBillingMessage packet;
    return packet;
}

SrvBillingMessage SrvBillingMessage::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvBillingMessage(reader);
}

std::unique_ptr<SrvBillingMessage> SrvBillingMessage::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvBillingMessage>(reader);
}

void SrvBillingMessage::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(functionType)) {
        return;
    }
    if (!writer.set_uint32_t(payFlag)) {
        return;
    }
}

constexpr size_t SrvBillingMessage::size() {
    size_t size = 0;
    size += sizeof(uint16_t);
    size += sizeof(uint32_t);
    return size;
}

