#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>
#include <vector>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKLC_LOGIN_REPLY, SrvLoginReply)
REGISTER_SEND_PACKET(ePacketType::PAKLC_LOGIN_REPLY, SrvLoginReply)
class SrvLoginReply : public CRosePacket {
	public:
		SrvLoginReply();
		SrvLoginReply(CRoseReader reader);
	private:
		SrvLoginReply(LoginReply::eResult result, uint16_t right, uint16_t type);
	public:

		virtual ~SrvLoginReply() = default;

		LoginReply::eResult result() const;
		SrvLoginReply& set_result(LoginReply::eResult);
		uint16_t right() const;
		SrvLoginReply& set_right(uint16_t);
		uint16_t type() const;
		SrvLoginReply& set_type(uint16_t);
		const std::vector<LoginReply::ServerInfo>& serversInfo() const;
		SrvLoginReply& set_serversInfo(const std::vector<LoginReply::ServerInfo>&);
		SrvLoginReply& add_serverinfo(const LoginReply::ServerInfo&);

		static SrvLoginReply create(LoginReply::eResult result, uint16_t right, uint16_t type);
		static SrvLoginReply create(uint8_t *buffer);
		static std::unique_ptr<SrvLoginReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		LoginReply::eResult result_;
		uint16_t right_;
		uint16_t type_;
		std::vector<LoginReply::ServerInfo> serversInfo_;
};

}