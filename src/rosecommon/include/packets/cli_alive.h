#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon
{
  REGISTER_RECV_PACKET(ePacketType::PAKCS_ALIVE, CliAlive)

  class CliAlive : public CRosePacket {
  public:
    CliAlive();
    CliAlive(uint8_t buffer[MAX_PACKET_SIZE]);

    virtual ~CliAlive() = default;

  protected:
    virtual void pack() override;
  };
}
