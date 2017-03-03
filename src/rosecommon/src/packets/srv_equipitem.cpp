#include "srv_equipitem.h"

namespace RoseCommon
{
  SrvEquipItem::SrvEquipItem() : CRosePacket( ePacketType::PAKWC_EQUIP_ITEM ) {}

  SrvEquipItem::SrvEquipItem(Entity entity, int16_t slot) : CRosePacket( ePacketType::PAKWC_EQUIP_ITEM ),
                                                            entity_( entity ),
                                                            slot_( slot ) {}

  Entity SrvEquipItem::entity() const {
    return entity_;
  }

  int16_t SrvEquipItem::slot() const {
    return slot_;
  }

  void SrvEquipItem::pack() {
    auto basicInfo = entity_.component<BasicInfo>();
    auto inventory = entity_.component<Inventory>();

    *this << basicInfo->id_;
    *this << slot_;
    *this << inventory->items_[slot_].getVisible();
  }
}
