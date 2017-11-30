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
 * \file item
 * \brief this file contains the \s Item class
 *
 * \author L3nn0x
 * \date october 2016
 */
#pragma once
#include "iserialize.h"
#include "components/lua.h"
#include "itemapi.h"

namespace RoseCommon {

/*!
 * \class Item
 * \brief Describes an item.
 * \sa ISerialize
 *
 * \author L3nn0x
 * \date october 2016
 */
struct Item : public ISerialize {
    enum Type {
        WEARABLE = 0,
        CONSUMABLE = 1,
        ETC = 2,
        RIDING = 3
    };

    Item();
    template <typename T>
    Item(const T& row) : Item() {
      loadFromRow(row);
    }

    Item(Item&& other) : type_(std::move(other.type_)),
                        id_(std::move(other.id_)),
                        isCreated_(std::move(other.isCreated_)),
                        gemOpt_(std::move(other.gemOpt_)),
                        durability_(std::move(other.durability_)),
                        life_(std::move(other.life_)),
                        hasSocket_(std::move(other.hasSocket_)),
                        isAppraised_(std::move(other.isAppraised_)),
                        refine_(std::move(other.refine_)),
                        count_(std::move(other.count_)),
                        isStackable_(std::move(other.isStackable_)),
                        lua_(std::move(other.lua_)) {}

    Item& operator=(Item&& other) { Item tmp(std::move(other)); std::swap(*this, tmp); return *this; }

    virtual ~Item() = default;

    virtual uint32_t getVisible() const override;
    virtual uint16_t getHeader() const override;
    virtual uint32_t getData() const override;

    template <typename T>
    void loadFromRow(const T& row) {
        id_ = row.itemid;
        type_ = row.itemtype;
        count_ = row.amount;
        refine_ = row.refine;
        gemOpt_ = row.gemOpt;
        hasSocket_ = row.socket;
        life_ = 1000; // FIXME : placeholder
        durability_ = 10; // FIXME : placeholder
        isAppraised_ = true; // FIXME : placeholder
    }

    template <typename U, typename T>
    void commitToUpdate(T& update) const {
      U inv;
      update.assignments.add(inv.itemid = id_);
      update.assignments.add(inv.itemtype = type_);
      update.assignments.add(inv.amount = count_);
      update.assignments.add(inv.refine = refine_);
      update.assignments.add(inv.gemOpt = gemOpt_);
      update.assignments.add(inv.socket = static_cast<int>(hasSocket_));
    }

    template <typename U, typename T>
    void commitToInsert(T& insert) const {
      U inv;
      insert.insert_list.add(inv.itemid = id_);
      insert.insert_list.add(inv.itemtype = type_);
      insert.insert_list.add(inv.amount = count_);
      insert.insert_list.add(inv.refine = refine_);
      insert.insert_list.add(inv.gemOpt = gemOpt_);
      insert.insert_list.add(inv.socket = static_cast<int>(hasSocket_));
    }

    operator bool() const {
      return id_ != 0;
    }

    bool operator==(const Item& other) const {
      return type_ == other.type_ && id_ == other.id_ && isCreated_ == other.isCreated_ &&
             gemOpt_ == other.gemOpt_ && durability_ == other.durability_ && life_ == other.life_ &&
             hasSocket_ == other.hasSocket_ && isAppraised_ == other.isAppraised_ && refine_ == other.refine_ &&
             count_ == other.count_ && isStackable_ == other.isStackable_;
    }

    bool operator!=(const Item& other) const {
      return !(*this == other);
    }

    uint8_t type_;
    uint16_t id_;
    bool isCreated_;
    uint16_t gemOpt_;
    uint8_t durability_;
    uint16_t life_;
    bool hasSocket_;
    bool isAppraised_;
    uint8_t refine_;
    uint32_t count_;
    bool isStackable_;

    uint16_t atk_;
    uint16_t def_;
    uint16_t range_;
 
    LuaComponent<ItemAPI> lua_;
};

}
