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
		static std::unique_ptr<SrvJoinServerReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		JoinServerReply::Result result_;
		uint32_t id_;
		uint32_t payFlag_= 0;
};

}