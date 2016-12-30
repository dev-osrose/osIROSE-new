#include "systems/inventorysystem.h"
#include "cmapserver.h"
#include "cmapclient.h"

using namespace Systems;
using namespace RoseCommon;

InventorySystem::InventorySystem(SystemManager &manager) : System(manager) {
    manager.registerDispatcher(ePacketType::PAKCS_EQUIP_ITEM, &InventorySystem::processEquip);
}

void InventorySystem::update(EntityManager&, double) {}

uint8_t InventorySystem::findNextEmptySlot(Entity entity) {
    if (!entity || !entity.component<Inventory>())
        return 0;
    const auto &inventory = entity.component<Inventory>()->items_;
    for (size_t i = Inventory::MAX_EQUIP_ITEMS; i < Inventory::maxItems; ++i)
        if (!inventory[i].id_)
            return i;
    return 0;
}

bool InventorySystem::swapItems(Entity entity, uint8_t &a, uint8_t &b) {
    if (!entity || !entity.component<Inventory>())
        return false;
    if (a >= Inventory::maxItems || b >= Inventory::maxItems)
        return false;
    if (a == b || (!a && !b))
        return false;
    if (!a)
        a = findNextEmptySlot(entity);
    if (!b)
        b = findNextEmptySlot(entity);
    auto inventory = entity.component<Inventory>();
    std::swap(inventory->items_[a], inventory->items_[b]);
    return true;
}

void InventorySystem::processEquip(CMapClient *client, Entity entity, const RoseCommon::CliEquipItem &packet) {
    logger_->trace("InventorySystem::processEquip");
    if (packet.slotTo() < 1 || packet.slotTo() >= Inventory::MAX_EQUIP_ITEMS) {
        logger_->warn("When requesting to change equipped item, the destination wasn't good");
        return;
    }
    uint8_t to = packet.slotTo();
    uint8_t from = packet.slotFrom();
    if (!swapItems(entity, to, from)) {
        logger_->warn("There was an error while swapping items for client {}", getId(entity));
        return;
    }
    CMapServer::SendPacket(client, CMapServer::eSendType::EVERYONE,
            *makePacket<ePacketType::PAKWC_EQUIP_ITEM>(entity, packet.slotTo()));
    auto list = { to, from };
    client->Send(*makePacket<ePacketType::PAKWC_SET_ITEM>(entity, list));
}
