#include "isc_shutdown.h"
#include "throwassert.h"

namespace RoseCommon {

IscShutdown::IscShutdown() : CRosePacket(ePacketType::ISC_SHUTDOWN) {}

IscShutdown::IscShutdown(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::ISC_SHUTDOWN, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); serverType_ = static_cast<Isc::ServerType>(_tmp); }
	reader.get_int32_t(id_);
}

IscShutdown::IscShutdown(Isc::ServerType serverType, int32_t id) : CRosePacket(ePacketType::ISC_SHUTDOWN), serverType_(serverType), id_(id) {
}

Isc::ServerType IscShutdown::serverType() const { return serverType_; }

IscShutdown& IscShutdown::set_serverType(Isc::ServerType data) { serverType_ = data; return *this; }

int32_t IscShutdown::id() const { return id_; }

IscShutdown& IscShutdown::set_id(int32_t data) { id_ = data; return *this; }


void IscShutdown::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(serverType_);
	writer.set_int32_t(id_);
}

uint16_t IscShutdown::get_size() const {
	uint16_t size = 0;
	size += sizeof(serverType_);
	size += sizeof(id_);
	return size;
}


IscShutdown IscShutdown::create(Isc::ServerType serverType, int32_t id) {


	return IscShutdown(serverType, id);
}

IscShutdown IscShutdown::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return IscShutdown(reader);
}

}