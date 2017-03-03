#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon
{
  REGISTER_SEND_PACKET(ePacketType::PAKSS_ACCEPT_REPLY, SrvAcceptReply)

  class SrvAcceptReply : public CRosePacket {
  public:
    SrvAcceptReply();
    SrvAcceptReply(uint32_t randValue, uint8_t result = 0x02);

    virtual ~SrvAcceptReply() = default;

    uint8_t result() const;
    uint32_t randValue() const;

  protected:
    virtual void pack() override;

  private:
    uint8_t result_;
    uint32_t randValue_;
  };
}
