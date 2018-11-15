#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCC_SWITCH_SERVER, SrvSwitchServer)
REGISTER_SEND_PACKET(ePacketType::PAKCC_SWITCH_SERVER, SrvSwitchServer)
class SrvSwitchServer : public CRosePacket {
	public:
		SrvSwitchServer();
		SrvSwitchServer(CRoseReader reader);
	private:
		SrvSwitchServer(uint16_t port, uint32_t sessionId, uint32_t sessionSeed, const std::string& ip);
	public:

		virtual ~SrvSwitchServer() = default;

		uint16_t port() const;
		SrvSwitchServer& set_port(uint16_t);
		uint32_t sessionId() const;
		SrvSwitchServer& set_sessionId(uint32_t);
		uint32_t sessionSeed() const;
		SrvSwitchServer& set_sessionSeed(uint32_t);
		const std::string& ip() const;
		SrvSwitchServer& set_ip(const std::string&);

		static SrvSwitchServer create(uint16_t port, uint32_t sessionId, uint32_t sessionSeed, const std::string& ip);
		static SrvSwitchServer create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t port_;
		uint32_t sessionId_;
		uint32_t sessionSeed_;
		std::string ip_;
};

}