#include "isc_shutdown.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> IscShutdown::init = RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]>(ePacketType::ISC_SHUTDOWN, &createPacket<IscShutdown>);

IscShutdown::IscShutdown() : CRosePacket(ePacketType::ISC_SHUTDOWN) {}

IscShutdown::IscShutdown(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::ISC_SHUTDOWN, "Not the right packet: " << to_underlying(type()));
  *this >> serverType_;
  *this >> id_;
}

IscShutdown::IscShutdown(Isc::ServerType serverType, int32_t id) : CRosePacket(ePacketType::ISC_SHUTDOWN), serverType_(serverType), id_(id) {}

  Isc::ServerType IscShutdown::serverType() const {
    return serverType_;
  }

  int32_t IscShutdown::id() const {
    return id_;
  }

void IscShutdown::pack() {
  *this << serverType_;
  *this << id_;
}

}
