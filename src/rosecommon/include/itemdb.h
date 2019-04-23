// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file ItemDB
 * \brief this file contains the \s ItemDatabase class which loads all of the base item data
 *
 * \author Raven
 * \date August 2017
 */
#pragma once
#include <vector>
#include <unordered_map>
#include <tuple>
#include <functional>

#include "singleton.h"
#include "hash.h"
#include "dataconsts.h"

namespace RoseCommon
{
  /*!
  * \class ItemDef
  * \brief Describes an item.
  * \sa
  *
  * \author Raven
  * \date August 2016
  */

  struct ItemDef {
    ItemType type;
    uint8_t subtype;
    uint16_t id;
    uint32_t buy_price;
    uint32_t sell_price;
    uint16_t weight;
    bool is_stackable;

    /// Damage
    uint16_t atk;
    uint16_t def;
    uint16_t range;
    uint8_t slots;
    uint8_t level;

    // Model id
    uint32_t view_id;

    std::string name;
    std::string desc;
    std::string script;

    ItemDef();
  };

  /*!
   * \class ItemDatabase
   * \brief Base item database manager class.
   * \sa ItemDef
   *
   * \author Raven
   * \date August 2016
   */
  class ItemDatabase : public Singleton<ItemDatabase> {
    friend Singleton<ItemDatabase>;
  public:
    const ItemDef& getItemDef(uint8_t type, uint16_t id) const;

    bool itemExists(uint8_t type, uint16_t id) const;

    void store_lua(std::function<void(uint8_t type, uint16_t id, const std::string& lua)> f) const;

  private:
    void initialize();
    ItemDatabase() { initialize(); }
    virtual ~ItemDatabase() = default;

    using key_t = std::tuple<uint8_t, uint16_t>;

    std::unordered_map<key_t, ItemDef, Core::tuple_hash> _database;
  };
}
