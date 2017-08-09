#include "isc_shutdown.h"
#include "throwassert.h"

namespace RoseCommon {

IscShutdown::IscShutdown() : CRosePacket(ePacketType::ISC_SHUTDOWN) {}

IscShutdown::IscShutdown(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::ISC_SHUTDOWN, "Not the right packet: " << to_underlying(type()));
	*this >> type_;
	*this >> id_;
}

IscShutdown::IscShutdown(Isc::ServerType type, int32_t id) : CRosePacket(ePacketType::ISC_SHUTDOWN), type_(type), id_(id) {}

Isc::ServerType IscShutdown::serverType() const {
	return type_;
}

int32_t IscShutdown::id() const {
	return id_;
}


void IscShutdown::pack() {
	*this << to_underlying(type_);
	*this << id_;
}

}
