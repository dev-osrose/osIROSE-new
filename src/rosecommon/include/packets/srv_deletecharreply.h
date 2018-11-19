#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCC_DELETE_CHAR_REPLY, SrvDeleteCharReply)
REGISTER_SEND_PACKET(ePacketType::PAKCC_DELETE_CHAR_REPLY, SrvDeleteCharReply)
class SrvDeleteCharReply : public CRosePacket {
	public:
		SrvDeleteCharReply();
		SrvDeleteCharReply(CRoseReader reader);
	private:
		SrvDeleteCharReply(uint32_t remainingTime, std::string name);
	public:

		virtual ~SrvDeleteCharReply() = default;

		uint32_t remainingTime() const;
		SrvDeleteCharReply& set_remainingTime(uint32_t);
		std::string name() const;
		SrvDeleteCharReply& set_name(std::string);

		static SrvDeleteCharReply create(uint32_t remainingTime, std::string name);
		static SrvDeleteCharReply create(uint8_t *buffer);
		static std::unique_ptr<SrvDeleteCharReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint32_t remainingTime_;
		std::string name_;
};

}