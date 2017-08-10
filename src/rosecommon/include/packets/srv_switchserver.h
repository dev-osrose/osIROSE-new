#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKCC_SWITCH_SERVER, SrvSwitchServer)
class SrvSwitchServer : public CRosePacket {
	public:
		SrvSwitchServer();
		SrvSwitchServer(uint8_t buffer[MAX_PACKET_SIZE]);
		SrvSwitchServer(uint16_t port, uint32_t sessionId, uint32_t sessionSeed, const std::string &ip);

		virtual ~SrvSwitchServer() = default;

		uint16_t port() const;
		uint32_t sessionId() const;
		uint32_t sessionSeed() const;
		std::string &ip();
		const std::string &ip() const;

	protected:
		virtual void pack() override;

	private:
		uint16_t port_;
		uint32_t sessionId_;
		uint32_t sessionSeed_;
		std::string ip_;
};

}
