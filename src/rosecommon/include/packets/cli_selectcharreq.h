#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_SELECT_CHAR_REQ, CliSelectCharReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_SELECT_CHAR_REQ, CliSelectCharReq)
class CliSelectCharReq : public CRosePacket {
	public:
		CliSelectCharReq();
		CliSelectCharReq(CRoseReader reader);
	private:
		CliSelectCharReq(uint8_t charId, uint8_t runMode, uint8_t rideMode, const std::string& name);
	public:

		virtual ~CliSelectCharReq() = default;

		uint8_t charId() const;
		CliSelectCharReq& set_charId(uint8_t);
		uint8_t runMode() const;
		CliSelectCharReq& set_runMode(uint8_t);
		uint8_t rideMode() const;
		CliSelectCharReq& set_rideMode(uint8_t);
		const std::string& name() const;
		CliSelectCharReq& set_name(const std::string&);

		static CliSelectCharReq create(uint8_t charId, uint8_t runMode, uint8_t rideMode, const std::string& name);
		static CliSelectCharReq create(uint8_t *buffer);
		static std::unique_ptr<CliSelectCharReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint8_t charId_;
		uint8_t runMode_;
		uint8_t rideMode_;
		std::string name_;
};

}