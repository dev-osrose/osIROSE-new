#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_CHANGE_MAP_REQ, CliChangeMapReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_CHANGE_MAP_REQ, CliChangeMapReq)
class CliChangeMapReq : public CRosePacket {
	public:
		CliChangeMapReq();
		CliChangeMapReq(CRoseReader reader);
	private:
		CliChangeMapReq(uint8_t weightRate, uint16_t z);
	public:

		virtual ~CliChangeMapReq() = default;

		uint8_t weightRate() const;
		CliChangeMapReq& set_weightRate(uint8_t);
		uint16_t z() const;
		CliChangeMapReq& set_z(uint16_t);

		static CliChangeMapReq create(uint8_t weightRate, uint16_t z);
		static CliChangeMapReq create(uint8_t *buffer);
		static std::unique_ptr<CliChangeMapReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint8_t weightRate_;
		uint16_t z_;
};

}