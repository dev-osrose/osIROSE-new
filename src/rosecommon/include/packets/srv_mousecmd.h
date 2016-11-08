#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_MOUSE_CMD, SrvMouseCmd)
class SrvMouseCmd : public CRosePacket {
	public:
		SrvMouseCmd();

		SrvMouseCmd(uint16_t sourceId, uint16_t destId, uint16_t dist, float x, float y, uint16_t z);

		virtual ~SrvMouseCmd() = default;

		uint16_t &sourceId();
		uint16_t &destId();
		uint16_t &dist();
		float &x();
		float &y();
		uint16_t &z();

    protected:
        virtual void pack() override;

	private:
		uint16_t sourceId_;
		uint16_t destId_;
		uint16_t dist_;
		float x_;
		float y_;
		uint16_t z_;
};

}
