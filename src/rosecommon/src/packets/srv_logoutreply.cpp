#include "srv_logoutreply.h"

namespace RoseCommon
{
  SrvLogoutReply::SrvLogoutReply() : CRosePacket( ePacketType::PAKWC_LOGOUT_REPLY ) {}

  SrvLogoutReply::SrvLogoutReply(uint16_t waitTime) : CRosePacket( ePacketType::PAKWC_LOGOUT_REPLY ),
                                                      waitTime_( waitTime ) {}

  uint16_t SrvLogoutReply::waitTime() const {
    return waitTime_;
  }

  void SrvLogoutReply::pack() {
    *this << waitTime_;
  }
}
