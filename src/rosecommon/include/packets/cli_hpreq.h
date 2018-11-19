#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_HP_REQ, CliHpReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_HP_REQ, CliHpReq)
class CliHpReq : public CRosePacket {
	public:
		CliHpReq();
		CliHpReq(CRoseReader reader);
	private:
		CliHpReq(uint16_t targetId);
	public:

		virtual ~CliHpReq() = default;

		uint16_t targetId() const;
		CliHpReq& set_targetId(uint16_t);

		static CliHpReq create(uint16_t targetId);
		static CliHpReq create(uint8_t *buffer);
		static std::unique_ptr<CliHpReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t targetId_;
};

}