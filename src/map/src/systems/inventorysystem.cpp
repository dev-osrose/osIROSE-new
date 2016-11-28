#include "systems/inventorysystem.h"
#include "cmapserver.h"

using namespace Systems;
using namespace RoseCommon;

InventorySystem::InventorySystem(SystemManager &manager) : System(manager) {
    manager.registerDispatcher(ePacketType::PAKCS_EQUIP_ITEM, &InventorySystem::processEquip);
}

void InventorySystem::update(EntityManager&, double) {}

bool InventorySystem::swapItems(Entity entity, uint8_t a, uint8_t b) {
    if (!entity || !entity.component<Inventory>())
        return false;
    if (a >= Inventory::maxItems || b >= Inventory::maxItems)
        return false;
    auto inventory = entity.component<Inventory>();
    auto tmp = inventory->items_[a];
    inventory->items_[a] = inventory->items_[b];
    inventory->items_[b] = tmp;
    return true;
}

void InventorySystem::processEquip(CMapClient *client, Entity entity, const RoseCommon::CliEquipItem &packet) {
    logger_->trace("InventorySystem::processEquip");
    if (packet.slotTo() < 1 || packet.slotTo() >= Inventory::MAX_EQUIP_ITEMS) {
        logger_->warn("When requesting to change equipped item, the destination wasn't good");
        return;
    }
    logger_->info("Swaping items from slot {} to {}", packet.slotFrom(), packet.slotTo());
    if (!swapItems(entity, packet.slotTo(), packet.slotFrom()))
        return;
    CMapServer::SendPacket(client, CMapServer::eSendType::EVERYONE,
            *makePacket<ePacketType::PAKWC_EQUIP_ITEM>(entity.component<BasicInfo>()->id_,
                packet.slotTo(),
                entity.component<Inventory>()->items_[packet.slotTo()]));
}
