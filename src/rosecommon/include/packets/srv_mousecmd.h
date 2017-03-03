#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon
{
  REGISTER_SEND_PACKET(ePacketType::PAKWC_MOUSE_CMD, SrvMouseCmd)

  class SrvMouseCmd : public CRosePacket {
  public:
    SrvMouseCmd();
    SrvMouseCmd(Entity entity);

    virtual ~SrvMouseCmd() = default;

    Entity entity() const;

  protected:
    virtual void pack() override;

  private:
    Entity entity_;
  };
}
