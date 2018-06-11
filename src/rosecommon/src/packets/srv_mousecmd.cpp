#include "srv_mousecmd.h"

namespace RoseCommon {

SrvMouseCmd::SrvMouseCmd() : CRosePacket(ePacketType::PAKWC_MOUSE_CMD) {}

SrvMouseCmd::SrvMouseCmd(Entity entity) : CRosePacket(ePacketType::PAKWC_MOUSE_CMD), entity_(entity) {}

Entity SrvMouseCmd::entity() const {
  return entity_;
}


void SrvMouseCmd::pack() {
  auto basicInfo = entity_.component<BasicInfo>();
  auto destination = entity_.component<Destination>();
  auto position = entity_.component<Position>();

  *this << basicInfo->id_;
  *this << basicInfo->target_;
  *this << destination->dist_;
  *this << destination->x_;
  *this << destination->y_;
  *this << position->z_;

}

}
