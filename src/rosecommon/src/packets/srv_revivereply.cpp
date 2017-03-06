#include "srv_revivereply.h"

namespace RoseCommon {

SrvReviveReply::SrvReviveReply() : CRosePacket(ePacketType::PAKWC_REVIVE_REPLY) {}

SrvReviveReply::SrvReviveReply(uint16_t mapId) : CRosePacket(ePacketType::PAKWC_REVIVE_REPLY), mapId_(mapId) {}

uint16_t SrvReviveReply::mapId() const {
	return mapId_;
}


void SrvReviveReply::pack() {
	*this << mapId_;
}

}