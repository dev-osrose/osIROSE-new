#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKLC_SRV_SELECT_REPLY, SrvSrvSelectReply)
REGISTER_SEND_PACKET(ePacketType::PAKLC_SRV_SELECT_REPLY, SrvSrvSelectReply)
class SrvSrvSelectReply : public CRosePacket {
	public:
		SrvSrvSelectReply();
		SrvSrvSelectReply(CRoseReader reader);
	private:
		SrvSrvSelectReply(SrvSelectReply::Result result, uint32_t sessionId, uint32_t cryptVal, const std::string& ip, uint16_t port);
	public:

		virtual ~SrvSrvSelectReply() = default;

		SrvSelectReply::Result result() const;
		SrvSrvSelectReply& set_result(SrvSelectReply::Result);
		uint32_t sessionId() const;
		SrvSrvSelectReply& set_sessionId(uint32_t);
		uint32_t cryptVal() const;
		SrvSrvSelectReply& set_cryptVal(uint32_t);
		const std::string& ip() const;
		SrvSrvSelectReply& set_ip(const std::string&);
		uint16_t port() const;
		SrvSrvSelectReply& set_port(uint16_t);

		static SrvSrvSelectReply create(SrvSelectReply::Result result, uint32_t sessionId, uint32_t cryptVal, const std::string& ip, uint16_t port);
		static SrvSrvSelectReply create(uint8_t *buffer);
		static std::unique_ptr<SrvSrvSelectReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		SrvSelectReply::Result result_;
		uint32_t sessionId_;
		uint32_t cryptVal_;
		std::string ip_;
		uint16_t port_;
};

}