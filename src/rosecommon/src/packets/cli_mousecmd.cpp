#include "cli_mousecmd.h"
#include "throwassert.h"

namespace RoseCommon {

CliMouseCmd::CliMouseCmd() : CRosePacket(ePacketType::PAKCS_MOUSE_CMD) {}

CliMouseCmd::CliMouseCmd(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_MOUSE_CMD, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(targetId_);
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_uint16_t(z_);
}

CliMouseCmd::CliMouseCmd(uint16_t targetId, float x, float y, uint16_t z) : CRosePacket(ePacketType::PAKCS_MOUSE_CMD), targetId_(targetId), x_(x), y_(y), z_(z) {
}

uint16_t CliMouseCmd::targetId() const { return targetId_; }

CliMouseCmd& CliMouseCmd::set_targetId(uint16_t data) { targetId_ = data; return *this; }

float CliMouseCmd::x() const { return x_; }

CliMouseCmd& CliMouseCmd::set_x(float data) { x_ = data; return *this; }

float CliMouseCmd::y() const { return y_; }

CliMouseCmd& CliMouseCmd::set_y(float data) { y_ = data; return *this; }

uint16_t CliMouseCmd::z() const { return z_; }

CliMouseCmd& CliMouseCmd::set_z(uint16_t data) { z_ = data; return *this; }


void CliMouseCmd::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(targetId_);
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_uint16_t(z_);
}

uint16_t CliMouseCmd::get_size() const {
	uint16_t size = 0;
	size += sizeof(targetId_);
	size += sizeof(x_);
	size += sizeof(y_);
	size += sizeof(z_);
	return size;
}


CliMouseCmd CliMouseCmd::create(uint16_t targetId, float x, float y, uint16_t z) {


	return CliMouseCmd(targetId, x, y, z);
}

CliMouseCmd CliMouseCmd::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliMouseCmd(reader);
}

}