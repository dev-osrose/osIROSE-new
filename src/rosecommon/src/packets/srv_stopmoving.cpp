#include "srv_stopmoving.h"

namespace RoseCommon
{
  SrvStopMoving::SrvStopMoving() : CRosePacket( ePacketType::PAKWC_STOP_MOVING ) {}

  SrvStopMoving::SrvStopMoving(Entity entity) : CRosePacket( ePacketType::PAKWC_STOP_MOVING ),
                                                entity_( entity ) {}

  Entity SrvStopMoving::entity() const {
    return entity_;
  }

  void SrvStopMoving::pack() {
    auto position = entity_.component<Position>();
    auto basicInfo = entity_.component<BasicInfo>();

    *this << basicInfo->id_;
    *this << position->x_;
    *this << position->y_;
    *this << position->z_;

  }
}
