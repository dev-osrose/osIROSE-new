#include "systems/inventorysystem.h"
#include "cli_dropitem.h"
#include "cli_equipitem.h"
#include "cmapclient.h"
#include "cmapserver.h"
#include "itemapi.h"
#include "itemdb.h"
#include "makevector.h"
#include "srv_dropitem.h"
#include "srv_equipitem.h"
#include "srv_setitem.h"
#include "systems/luasystem.h"

using namespace Systems;
using namespace RoseCommon;

InventorySystem::InventorySystem(SystemManager &manager) : System(manager) {
  manager.registerDispatcher(ePacketType::PAKCS_EQUIP_ITEM, &InventorySystem::processEquip);
  manager.registerDispatcher(ePacketType::PAKCS_DROP_ITEM, &InventorySystem::dropItem);
}

void InventorySystem::update(EntityManager &, std::chrono::milliseconds) {}

uint8_t InventorySystem::findNextEmptySlot(Entity entity) {
  if (!entity || !entity.component<Inventory>()) return 0;
  const auto &inventory = entity.component<Inventory>()->items_;
  for (size_t i = Inventory::MAX_EQUIP_ITEMS; i < Inventory::maxItems; ++i)
    if (!inventory[i].id_) return i;
  return 0;
}

bool InventorySystem::swapItems(Entity entity, uint8_t &a, uint8_t &b) {
  if (!entity || !entity.component<Inventory>()) return false;
  if (a >= Inventory::maxItems || b >= Inventory::maxItems) return false;
  if (a == b || (!a && !b)) return false;
  if (!a) a = findNextEmptySlot(entity);
  if (!b) b = findNextEmptySlot(entity);
  auto inventory = entity.component<Inventory>();
  std::swap(inventory->items_[a], inventory->items_[b]);
  return true;
}

void InventorySystem::processEquip(CMapClient &client, Entity entity, const RoseCommon::CliEquipItem &packet) {
  logger_->trace("InventorySystem::processEquip");
  if (packet.slotTo() < 1 || packet.slotTo() >= Inventory::MAX_EQUIP_ITEMS) {
    logger_->warn("When requesting to change equipped item, the destination wasn't good");
    return;
  }
  uint8_t to = packet.slotTo();
  uint8_t from = packet.slotFrom();

  int unequip = -1;
  int equip = -1;
  auto inv = entity.component<Inventory>();
  // if the slot we are moving to was an equipped item, we unequip it
  if (to < Inventory::MAX_EQUIP_ITEMS && inv->items_[to])
    unequip = static_cast<bool>(entity.component<Inventory>()->items_[to].lua_.onUnequip(&entity));
  // if the slot we are moving to is an equipped item, we equip it
  if (to < Inventory::MAX_EQUIP_ITEMS && inv->items_[from])
    equip = static_cast<bool>(entity.component<Inventory>()->items_[from].lua_.onEquip(&entity));

  if (!equip || !unequip) return;  // we couldn't equip / unequip an item

  if (!swapItems(entity, to, from)) {
    logger_->warn("There was an error while swapping items for client {}", getId(entity));
    return;
  }
  manager_.send(entity, CMapServer::eSendType::NEARBY, SrvEquipItem::create(entity, packet.slotTo()));
  auto sendPacket = SrvSetItem::create();
  sendPacket.add_item(SetItem::Item(inv->items_[to].id_, inv->items_[to].getHeader(), inv->items_[to].getData()));
  sendPacket.add_item(SetItem::Item(inv->items_[from].id_, inv->items_[from].getHeader(), inv->items_[from].getData()));
  client.send(std::move(sendPacket));
}

void InventorySystem::dropItem(CMapClient &client, Entity entity, const RoseCommon::CliDropItem &packet) {
  logger_->trace("InventorySystem::dropItem");
  if (packet.item() == 0 || packet.item() >= Inventory::maxItems) {
    logger_->warn("Wrong item to be dropped");
    return;
  }
  auto inv = entity.component<Inventory>();
  const uint32_t held = inv->items_[packet.item()].count_;
  uint32_t count = packet.quantity();
  if (held < count) {
    logger_->warn("The quantity is above what the character has");
    count = held;
  }
  Item droppedItem;
  if (count < held) {
    Item &item = inv->items_[packet.item()];
    item.count_ -= count;
    droppedItem = item;
    droppedItem.count_ = count;
    auto sendPacket = SrvSetItem::create();
    sendPacket.add_item(SetItem::Item(inv->items_[packet.item()].id_, inv->items_[packet.item()].getHeader(), inv->items_[packet.item()].getData()));
    client.send(std::move(sendPacket));
  } else {
    droppedItem = removeItem(entity, packet.item());
  }
  Entity item = manager_.buildItem(entity, std::move(droppedItem));
  manager_.send(entity, CMapServer::eSendType::NEARBY, SrvDropItem::create(item));
}

bool InventorySystem::addItem(Entity e, Item &&item) {
  uint8_t slot = findNextEmptySlot(e);
  if (!slot) return false;
  item.lua_.onPickup(&e);
  auto inv = e.component<Inventory>();
  inv->items_[slot] = std::move(item);

  auto client = getClient(e);

  auto sendPacket = SrvSetItem::create();
  sendPacket.add_item(SetItem::Item(inv->items_[slot].id_, inv->items_[slot].getHeader(), inv->items_[slot].getData()));
  client->send(std::move(sendPacket));
  return true;
}

Item InventorySystem::removeItem(Entity entity, uint8_t slot) {
  if (!slot || slot >= Inventory::maxItems) return {};
  auto inv = entity.component<Inventory>();
  Item item{std::move(inv->items_[slot])};
  inv->items_[slot] = {};
  item.lua_.onDrop(&entity);

  auto client = getClient(entity);
  auto sendPacket = SrvSetItem::create();
  sendPacket.add_item(SetItem::Item(inv->items_[slot].id_, inv->items_[slot].getHeader(), inv->items_[slot].getData()));
  client->send(std::move(sendPacket));

  if (slot < Inventory::MAX_EQUIP_ITEMS) {
    // if the item removed was equipped, we send the update to everybody in range
    // this can happen if the equipment is destroyed while in combat or something
    manager_.send(entity, CMapServer::eSendType::NEARBY, SrvEquipItem::create(entity, slot));
  }
  return item;
}

std::optional<Item> InventorySystem::buildItem(uint8_t type, uint16_t id, uint16_t life, bool isAppraised) {
  const auto &itemDb = ItemDatabase::getInstance();
  const auto def = itemDb.getItemDef(type, id);
  Item item{def};
  item.life_ = life;
  item.isAppraised_ = isAppraised;
  auto luaSystem = manager_.get<LuaSystem>();
  auto lua = luaSystem->loadScript<RoseCommon::ItemAPI>(def.script);
  if (!lua) return {};
  item.lua_ = std::move(lua.value());
  // const auto& env = item.lua_.getEnv();
  // TODO: get durability from lua env
  item.lua_.onInit();
  return std::make_optional(std::move(item));
}
