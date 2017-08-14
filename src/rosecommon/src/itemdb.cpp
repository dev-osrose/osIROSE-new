#include "itemdb.h"
#include "connection.h"

using namespace RoseCommon;

ItemDatabase::ItemDatabase() {
  for ( int index = 1; index < 15; ++index ) {
    _database[index].reserve( 1000 ); // We shouldn't need more then 1000 entries off the bat.
  }
}

ItemDatabase::~ItemDatabase() {
  _database.clear();
}

ItemDef::ItemDef(): type( 0 ),
                    subtype( 0 ),
                    id( 0 ),
                    buy_price( 0 ),
                    sell_price( 0 ),
                    weight( 0 ),
                    atk( 0 ),
                    def( 0 ),
                    range( 0 ),
                    slots( 0 ),
                    level( 0 ),
                    view_id( 0 ) { }

void ItemDatabase::initialize() {


  // Load the item db table
  auto conn = Core::connectionPool.getConnection( Core::osirose );
  Core::ItemDBTable itemdb;
  try {
    ItemDef newItem;
    for ( uint8_t itemType = 1; itemType < 15; ++itemType ) {
      for ( const auto& row : conn( sqlpp::select( sqlpp::all_of( itemdb ) ).from( itemdb ).where( itemdb.type == itemType ) ) ) {
        newItem.type = static_cast<uint8_t>(row.type);
        newItem.subtype = static_cast<uint8_t>(row.subtype);
        newItem.id = static_cast<uint16_t>(row.id);
        newItem.buy_price = static_cast<uint32_t>(row.priceBuy);
        newItem.sell_price = static_cast<uint32_t>(row.priceSell);
        newItem.weight = static_cast<uint16_t>(row.weight);
        newItem.atk = static_cast<uint16_t>(row.attack);
        newItem.def = static_cast<uint16_t>(row.defense);
        newItem.slots = static_cast<uint8_t>(row.slots);
        newItem.level = static_cast<uint8_t>(row.equipLevel);
        newItem.view_id = static_cast<uint32_t>(row.viewId);

        newItem.name = row.name;
        newItem.desc = row.desc;
        newItem.script = row.script;

        _database[itemType].push_back(newItem);
      }
    }
  }
  catch ( sqlpp::exception& ) { }
}
