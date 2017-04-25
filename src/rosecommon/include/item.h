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
    virtual ~Item() = default;

    virtual uint32_t getVisible() const override;
    virtual uint16_t getHeader() const override;
    virtual uint32_t getData() const override;

    virtual void saveToDatabase() const override;
    virtual void loadFromDatabase(Core::IRow&) override;

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
};

}
