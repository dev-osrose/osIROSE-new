#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include "isccommon.h"

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::ISC_SHUTDOWN, IscShutdown)
REGISTER_RECV_PACKET(ePacketType::ISC_SHUTDOWN, IscShutdown)
class IscShutdown : public CRosePacket {
	public:
		IscShutdown();
		IscShutdown(uint8_t buffer[MAX_PACKET_SIZE]);
		IscShutdown(Isc::ServerType type, int32_t id);

		virtual ~IscShutdown() = default;

    Isc::ServerType serverType() const;
		int32_t id() const;

	protected:
		virtual void pack() override;

	private:
    Isc::ServerType type_;
		int32_t id_;
};

}
