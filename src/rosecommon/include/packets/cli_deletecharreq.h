#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_DELETE_CHAR_REQ, CliDeleteCharReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_DELETE_CHAR_REQ, CliDeleteCharReq)
class CliDeleteCharReq : public CRosePacket {
	public:
		CliDeleteCharReq();
		CliDeleteCharReq(CRoseReader reader);
	private:
		CliDeleteCharReq(uint8_t charId, uint8_t isDelete, const std::string& name);
	public:

		virtual ~CliDeleteCharReq() = default;

		uint8_t charId() const;
		CliDeleteCharReq& set_charId(uint8_t);
		uint8_t isDelete() const;
		CliDeleteCharReq& set_isDelete(uint8_t);
		const std::string& name() const;
		CliDeleteCharReq& set_name(const std::string&);

		static CliDeleteCharReq create(uint8_t charId, uint8_t isDelete, const std::string& name);
		static CliDeleteCharReq create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint8_t charId_;
		uint8_t isDelete_;
		std::string name_;
};

}