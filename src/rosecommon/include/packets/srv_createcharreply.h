#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCC_CREATE_CHAR_REPLY, SrvCreateCharReply)
REGISTER_SEND_PACKET(ePacketType::PAKCC_CREATE_CHAR_REPLY, SrvCreateCharReply)
class SrvCreateCharReply : public CRosePacket {
	public:
		SrvCreateCharReply();
		SrvCreateCharReply(CRoseReader reader);
	private:
		SrvCreateCharReply(CreateCharReply::Result result);
	public:

		virtual ~SrvCreateCharReply() = default;

		CreateCharReply::Result result() const;
		SrvCreateCharReply& set_result(CreateCharReply::Result);
		uint8_t platinium() const;
		SrvCreateCharReply& set_platinium(uint8_t);

		static SrvCreateCharReply create(CreateCharReply::Result result);
		static SrvCreateCharReply create(uint8_t *buffer);
		static std::unique_ptr<SrvCreateCharReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		CreateCharReply::Result result_;
		uint8_t platinium_= 0;
};

}