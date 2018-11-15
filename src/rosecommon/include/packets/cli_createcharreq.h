#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_CREATE_CHAR_REQ, CliCreateCharReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_CREATE_CHAR_REQ, CliCreateCharReq)
class CliCreateCharReq : public CRosePacket {
	public:
		CliCreateCharReq();
		CliCreateCharReq(CRoseReader reader);
	private:
		CliCreateCharReq(uint8_t race, uint8_t stone, uint8_t hair, uint8_t face, uint8_t weapon, uint16_t zone, const std::string& name);
	public:

		virtual ~CliCreateCharReq() = default;

		uint8_t race() const;
		CliCreateCharReq& set_race(uint8_t);
		uint8_t stone() const;
		CliCreateCharReq& set_stone(uint8_t);
		uint8_t hair() const;
		CliCreateCharReq& set_hair(uint8_t);
		uint8_t face() const;
		CliCreateCharReq& set_face(uint8_t);
		uint8_t weapon() const;
		CliCreateCharReq& set_weapon(uint8_t);
		uint16_t zone() const;
		CliCreateCharReq& set_zone(uint16_t);
		const std::string& name() const;
		CliCreateCharReq& set_name(const std::string&);

		static CliCreateCharReq create(uint8_t race, uint8_t stone, uint8_t hair, uint8_t face, uint8_t weapon, uint16_t zone, const std::string& name);
		static CliCreateCharReq create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

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