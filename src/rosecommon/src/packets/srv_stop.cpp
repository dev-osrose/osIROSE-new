#include "srv_stop.h"

namespace RoseCommon
{
  SrvStop::SrvStop() : CRosePacket( ePacketType::PAKWC_STOP ) {}

  SrvStop::SrvStop(Entity entity) : CRosePacket( ePacketType::PAKWC_STOP ),
                                    entity_( entity ) {}

  Entity SrvStop::entity() const {
    return entity_;
  }

  void SrvStop::pack() {
    auto position = entity_.component<Position>();
    auto basicInfo = entity_.component<BasicInfo>();

    *this << basicInfo->id_;
    *this << position->x_;
    *this << position->y_;
    *this << position->z_;

  }
}
