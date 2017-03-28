#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_CREATE_CHAR_REQ, CliCreateCharReq)
class CliCreateCharReq : public CRosePacket {
	public:
		CliCreateCharReq();
		CliCreateCharReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliCreateCharReq(uint8_t race, uint8_t stone, uint8_t hair, uint8_t face, uint8_t weapon, uint16_t zone, const std::string &name);

		virtual ~CliCreateCharReq() = default;

		uint8_t race() const;
		uint8_t stone() const;
		uint8_t hair() const;
		uint8_t face() const;
		uint8_t weapon() const;
		uint16_t zone() const;
		std::string &name();
		const std::string &name() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t race_;
		uint8_t stone_;
		uint8_t hair_;
		uint8_t face_;
		uint8_t weapon_;
		uint16_t zone_;
		std::string name_;
};

}
