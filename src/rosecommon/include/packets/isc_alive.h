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
		static std::unique_ptr<IscAlive> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;
};

}