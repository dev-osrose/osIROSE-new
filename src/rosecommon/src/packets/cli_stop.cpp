#include "cli_stop.h"
#include "throwassert.h"

namespace RoseCommon {

CliStop::CliStop() : CRosePacket(ePacketType::PAKCS_STOP) {}

CliStop::CliStop(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_STOP, "Not the right packet: " << to_underlying(get_type()));
	reader.get_float(x_);
	reader.get_float(y_);
}

CliStop::CliStop(float x, float y) : CRosePacket(ePacketType::PAKCS_STOP), x_(x), y_(y) {
}

float CliStop::x() const { return x_; }

CliStop& CliStop::set_x(float data) { x_ = data; return *this; }

float CliStop::y() const { return y_; }

CliStop& CliStop::set_y(float data) { y_ = data; return *this; }


void CliStop::pack(CRoseBasePolicy& writer) const {
	writer.set_float(x_);
	writer.set_float(y_);
}

CliStop CliStop::create(float x, float y) {


	return CliStop(x, y);
}

CliStop CliStop::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliStop(reader);
}
std::unique_ptr<CliStop> CliStop::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliStop>(reader);
}

}