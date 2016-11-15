#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_CHANGE_MAP_REQ, CliChangeMapReq)
class CliChangeMapReq : public CRosePacket {
	public:
		CliChangeMapReq();
		CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliChangeMapReq(Entity entity);

		virtual ~CliChangeMapReq() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}