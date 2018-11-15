#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <isccommon.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::ISC_SHUTDOWN, IscShutdown)
REGISTER_SEND_PACKET(ePacketType::ISC_SHUTDOWN, IscShutdown)
class IscShutdown : public CRosePacket {
	public:
		IscShutdown();
		IscShutdown(CRoseReader reader);
	private:
		IscShutdown(Isc::ServerType serverType, int32_t id);
	public:

		virtual ~IscShutdown() = default;

		Isc::ServerType serverType() const;
		IscShutdown& set_serverType(Isc::ServerType);
		int32_t id() const;
		IscShutdown& set_id(int32_t);

		static IscShutdown create(Isc::ServerType serverType, int32_t id);
		static IscShutdown create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		Isc::ServerType serverType_;
		int32_t id_;
};

}