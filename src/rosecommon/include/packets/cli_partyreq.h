#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon
{
  REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_REQ, CliPartyReq)

  class CliPartyReq : public CRosePacket {
  public:
    enum Request : uint8_t {
      MAKE = 0,
      JOIN = 1,
      LEFT,
      CHANGE_OWNER,
      KICK = 0x81
    };

    CliPartyReq();
    CliPartyReq(uint8_t buffer[MAX_PACKET_SIZE]);
    CliPartyReq(Request request, uint32_t idXorTag);

    virtual ~CliPartyReq() = default;

    Request request() const;
    uint32_t idXorTag() const;

  protected:
    virtual void pack() override;

  private:
    Request request_;
    uint32_t idXorTag_;
  };
}
