#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <vector>

namespace RoseCommon
{
  REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_ITEM, SrvSetItem)

  class SrvSetItem : public CRosePacket {
  public:
    SrvSetItem();
    SrvSetItem(Entity entity, const std::vector<uint8_t>& list);

    virtual ~SrvSetItem() = default;

    Entity entity() const;
    const std::vector<uint8_t>& list() const;

  protected:
    virtual void pack() override;

  private:
    Entity entity_;
    std::vector<uint8_t> list_;
  };
}
