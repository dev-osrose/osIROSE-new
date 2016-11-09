#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKCC_DELETE_CHAR_REPLY, SrvDeleteCharReply)
class SrvDeleteCharReply : public CRosePacket {
	public:
		SrvDeleteCharReply();

		SrvDeleteCharReply(uint32_t remainingTime, const std::string &name);

		virtual ~SrvDeleteCharReply() = default;

		uint32_t &remainingTime();
		std::string &name();

	protected:
		virtual void pack() override;

	private:
		uint32_t remainingTime_;
		std::string name_;
};

}
