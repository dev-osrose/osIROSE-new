#include "itemdb.h"
#include "osiroseDatabase.h"
#include "connection.h"
#include "throwassert.h"

using namespace RoseCommon;

ItemDef::ItemDef(): type( 0 ),
                    subtype( 0 ),
                    id( 0 ),
                    item_type( ItemType::WEARABLE ),
                    buy_price( 0 ),
                    sell_price( 0 ),
                    weight( 0 ),
                    is_stackable( 0 ),
                    atk( 0 ),
                    def( 0 ),
                    range( 0 ),
                    slots( 0 ),
                    level( 0 ),
                    view_id( 0 ) { }

void ItemDatabase::initialize() {


  // Load the item db table
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::ItemDBTable itemdb{};
  try {
    ItemDef newItem;
    for ( const auto& row : conn( sqlpp::select( sqlpp::all_of( itemdb ) ).from( itemdb ).where(itemdb.type != 0).order_by(itemdb.id.asc()) ) ) {
      newItem.type = static_cast<uint8_t>(row.type);
      newItem.subtype = static_cast<uint8_t>(row.subtype);
      newItem.id = static_cast<uint16_t>(row.id);
      if (newItem.type == 14) {
          newItem.item_type = ItemType::RIDING;
      } else if (newItem.type < EquippedPosition::MAX_EQUIP_ITEMS) {
          newItem.item_type = ItemType::WEARABLE;
      } else if (newItem.type == 10) {
        newItem.item_type = ItemType::CONSUMABLE;
      } else {
        newItem.item_type = ItemType::ETC;
      }
      newItem.buy_price = static_cast<uint32_t>(row.priceBuy);
      newItem.sell_price = static_cast<uint32_t>(row.priceSell);
      newItem.weight = static_cast<uint16_t>(row.weight);
      newItem.is_stackable = newItem.item_type == ItemType::CONSUMABLE || newItem.item_type == ItemType::ETC;
      newItem.atk = static_cast<uint16_t>(row.attack);
      newItem.def = static_cast<uint16_t>(row.defense);
      newItem.slots = static_cast<uint8_t>(row.slots);
      newItem.level = static_cast<uint8_t>(row.equipLevel);
      newItem.view_id = static_cast<uint32_t>(row.viewId);
      
      newItem.name = row.name;
      newItem.desc = row.desc;
      newItem.script = row.script;
      
      _database[ItemDatabase::key_t{row.type, row.id}] = newItem;
    }
  }
  catch ( sqlpp::exception& ) { }
}

void ItemDatabase::store_lua(std::function<void(uint8_t type, uint16_t id, const std::string& lua)> f) const {
    for (const auto& it : _database) {
      f(std::get<0>(it.first), std::get<1>(it.first), it.second.script);
    }
}

bool ItemDatabase::itemExists(uint8_t type, uint16_t id) const {
    return _database.count(ItemDatabase::key_t{type, id}) != 0;
}

const ItemDef& ItemDatabase::getItemDef(uint8_t type, uint16_t id) const {
    throw_assert(_database.count(ItemDatabase::key_t{type, id}), "No default for item " << type << " " << id);
    return _database.at(ItemDatabase::key_t{type, id});
}
