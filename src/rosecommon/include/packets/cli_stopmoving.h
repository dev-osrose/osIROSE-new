#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_STOP_MOVING, CliStopMoving)
REGISTER_SEND_PACKET(ePacketType::PAKCS_STOP_MOVING, CliStopMoving)
class CliStopMoving : public CRosePacket {
	public:
		CliStopMoving();
		CliStopMoving(CRoseReader reader);
	private:
		CliStopMoving(float x, float y, int16_t z);
	public:

		virtual ~CliStopMoving() = default;

		float x() const;
		CliStopMoving& set_x(float);
		float y() const;
		CliStopMoving& set_y(float);
		int16_t z() const;
		CliStopMoving& set_z(int16_t);

		static CliStopMoving create(float x, float y, int16_t z);
		static CliStopMoving create(uint8_t *buffer);
		static std::unique_ptr<CliStopMoving> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		float x_;
		float y_;
		int16_t z_;
};

}