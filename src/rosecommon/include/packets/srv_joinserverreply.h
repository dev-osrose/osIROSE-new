#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKSC_JOIN_SERVER_REPLY, SrvJoinServerReply)
REGISTER_SEND_PACKET(ePacketType::PAKSC_JOIN_SERVER_REPLY, SrvJoinServerReply)
class SrvJoinServerReply : public CRosePacket {
	public:
		SrvJoinServerReply();
		SrvJoinServerReply(CRoseReader reader);
	private:
		SrvJoinServerReply(JoinServerReply::Result result, uint32_t id);
	public:

		virtual ~SrvJoinServerReply() = default;

		JoinServerReply::Result result() const;
		SrvJoinServerReply& set_result(JoinServerReply::Result);
		uint32_t id() const;
		SrvJoinServerReply& set_id(uint32_t);
		uint32_t payFlag() const;
		SrvJoinServerReply& set_payFlag(uint32_t);

		static SrvJoinServerReply create(JoinServerReply::Result result, uint32_t id);
		static SrvJoinServerReply create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		JoinServerReply::Result result_;
		uint32_t id_;
		uint32_t payFlag_= 0;
};

}