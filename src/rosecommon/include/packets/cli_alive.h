#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_ALIVE, CliAlive)
REGISTER_SEND_PACKET(ePacketType::PAKCS_ALIVE, CliAlive)
class CliAlive : public CRosePacket {
	public:
		CliAlive();
		CliAlive(CRoseReader reader);
	public:

		virtual ~CliAlive() = default;


		static CliAlive create();
		static CliAlive create(uint8_t *buffer);
		static std::unique_ptr<CliAlive> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;
};

}