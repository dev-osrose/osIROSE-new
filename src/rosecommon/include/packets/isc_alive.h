#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::ISC_ALIVE, IscAlive)
REGISTER_SEND_PACKET(ePacketType::ISC_ALIVE, IscAlive)
class IscAlive : public CRosePacket {
	public:
		IscAlive();
		IscAlive(CRoseReader reader);
	public:

		virtual ~IscAlive() = default;


		static IscAlive create();
		static IscAlive create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;
};

}