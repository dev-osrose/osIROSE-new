#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ, CliScreenShotTimeReq)
class CliScreenShotTimeReq : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t*> init;
	public:
		CliScreenShotTimeReq();
		CliScreenShotTimeReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliScreenShotTimeReq(uint16_t count);

		virtual ~CliScreenShotTimeReq() = default;

		uint16_t count() const;

	protected:
		virtual void pack() override;

	private:
		uint16_t count_;
};

}
