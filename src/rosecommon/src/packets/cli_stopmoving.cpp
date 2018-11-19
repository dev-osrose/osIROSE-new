#include "cli_stopmoving.h"
#include "throwassert.h"

namespace RoseCommon {

CliStopMoving::CliStopMoving() : CRosePacket(ePacketType::PAKCS_STOP_MOVING) {}

CliStopMoving::CliStopMoving(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_STOP_MOVING, "Not the right packet: " << to_underlying(get_type()));
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_int16_t(z_);
}

CliStopMoving::CliStopMoving(float x, float y, int16_t z) : CRosePacket(ePacketType::PAKCS_STOP_MOVING), x_(x), y_(y), z_(z) {
}

float CliStopMoving::x() const { return x_; }

CliStopMoving& CliStopMoving::set_x(float data) { x_ = data; return *this; }

float CliStopMoving::y() const { return y_; }

CliStopMoving& CliStopMoving::set_y(float data) { y_ = data; return *this; }

int16_t CliStopMoving::z() const { return z_; }

CliStopMoving& CliStopMoving::set_z(int16_t data) { z_ = data; return *this; }


void CliStopMoving::pack(CRoseBasePolicy& writer) const {
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_int16_t(z_);
}

CliStopMoving CliStopMoving::create(float x, float y, int16_t z) {


	return CliStopMoving(x, y, z);
}

CliStopMoving CliStopMoving::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliStopMoving(reader);
}
std::unique_ptr<CliStopMoving> CliStopMoving::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliStopMoving>(reader);
}

}