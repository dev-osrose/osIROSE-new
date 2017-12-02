#include "systems/inventorysystem.h"
#include "cmapserver.h"
#include "cmapclient.h"
#include "makevector.h"
#include "cli_equipitem.h"
#include "srv_equipitem.h"
#include "srv_setitem.h"
#include "itemdb.h"
#include "systems/luasystem.h"
#include "itemapi.h"

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

void InventorySystem::processEquip(CMapClient& client, Entity entity, const RoseCommon::CliEquipItem &packet) {
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
    client.send(*makePacket<ePacketType::PAKWC_SET_ITEM>(entity, Core::make_vector(to, from)));
}

bool InventorySystem::addItem(Entity e, RoseCommon::Item&& item) {
    uint8_t slot = findNextEmptySlot(e);
    if (!slot) return false;
    auto inv = e.component<Inventory>();
    inv->items_[slot] = std::move(item);

    auto client = getClient(e);

    client->send(*makePacket<ePacketType::PAKWC_SET_ITEM>(e, Core::make_vector(slot)));
    return true;
}

RoseCommon::Item InventorySystem::buildItem(uint8_t type, uint8_t subtype, uint16_t id) {
    auto &itemDb = ItemDatabase::getInstance();
    auto def = itemDb.getItemDef(type, subtype, id);
    RoseCommon::Item item(def);
    auto luaSystem = manager_.get<LuaSystem>();
    item.lua_ = luaSystem->loadScript<RoseCommon::ItemAPI>("");
    return item;
}
