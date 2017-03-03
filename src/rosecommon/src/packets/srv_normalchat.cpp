#include "srv_normalchat.h"

namespace RoseCommon
{
  SrvNormalChat::SrvNormalChat() : CRosePacket( ePacketType::PAKWC_NORMAL_CHAT ) {}

  SrvNormalChat::SrvNormalChat(uint16_t charId, const std::string& message) : CRosePacket( ePacketType::PAKWC_NORMAL_CHAT ),
                                                                              charId_( charId ),
                                                                              message_( message ) {}

  uint16_t SrvNormalChat::charId() const {
    return charId_;
  }

  std::string& SrvNormalChat::message() {
    return message_;
  }

  const std::string& SrvNormalChat::message() const {
    return message_;
  }

  void SrvNormalChat::pack() {
    *this << charId_;
    *this << message_;
  }
}
