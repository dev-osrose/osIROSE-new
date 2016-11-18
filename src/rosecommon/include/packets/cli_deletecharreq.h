#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_DELETE_CHAR_REQ, CliDeleteCharReq)
class CliDeleteCharReq : public CRosePacket {
	public:
		CliDeleteCharReq();
		CliDeleteCharReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliDeleteCharReq(uint8_t charId, uint8_t isDelete, const std::string &name);

		virtual ~CliDeleteCharReq() = default;

		uint8_t charId() const;
		uint8_t isDelete() const;
		std::string &name();
		const std::string &name() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t charId_;
		uint8_t isDelete_;
		std::string name_;
};

}