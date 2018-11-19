#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKSS_ACCEPT_REPLY, SrvAcceptReply)
REGISTER_SEND_PACKET(ePacketType::PAKSS_ACCEPT_REPLY, SrvAcceptReply)
class SrvAcceptReply : public CRosePacket {
	public:
		SrvAcceptReply();
		SrvAcceptReply(CRoseReader reader);
	private:
		SrvAcceptReply(uint8_t result, uint32_t randValue);
	public:

		virtual ~SrvAcceptReply() = default;

		uint8_t result() const;
		SrvAcceptReply& set_result(uint8_t);
		uint32_t randValue() const;
		SrvAcceptReply& set_randValue(uint32_t);

		static SrvAcceptReply create(uint8_t result, uint32_t randValue);
		static SrvAcceptReply create(uint8_t *buffer);
		static std::unique_ptr<SrvAcceptReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint8_t result_;
		uint32_t randValue_;
};

}