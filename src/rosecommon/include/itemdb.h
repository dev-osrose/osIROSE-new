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
#include <map>

#include "iserialize.h"
#include "singleton.h"

namespace RoseCommon
{
  struct ItemDef {
    uint8_t type;
    uint8_t subtype;
    uint16_t id;
    uint32_t buy_price;
    uint32_t sell_price;
    uint16_t weight;

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
   * \brief Describes an item.
   * \sa ISerialize
   *
   * \author L3nn0x
   * \date october 2016
   */
  class ItemDatabase {
  public:
    void initialize();

  //private:
    ItemDatabase();
    virtual ~ItemDatabase();

    std::map<uint8_t, std::vector<ItemDef>> _database;
  };
}
