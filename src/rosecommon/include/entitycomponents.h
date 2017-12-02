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
 * \file entitycomponents
 * \brief The file defining the entity component model
 *
 * \author L3nn0x
 * \date october 2016
 */

#ifndef _ENTITYCOMPONENTS_H_
# define _ENTITYCOMPONENTS_H_

#include "entityx.hh"

#include "components/socketconnector.h"
#include "components/basicinfo.h"
#include "components/stats.h"
#include "components/advancedinfo.h"
#include "components/graphics.h"
#include "components/charactergraphics.h"
#include "components/characterinfo.h"
#include "components/position.h"
#include "components/destination.h"
#include "components/statuseffects.h"
#include "components/skills.h"
#include "components/hotbar.h"
#include "components/ridingitems.h"
#include "components/bulletitems.h"
#include "components/inventory.h"
#include "components/party.h"
#include "components/wishlist.h"
#include "components/quests.h"

#include "entityapi.h"

using EntityManager = entityx::EntityX<entityx::DefaultStorage, entityx::FeatureFlags::OBSERVABLE,
      SocketConnector, BasicInfo, Stats, AdvancedInfo, CharacterInfo, Graphics,
      CharacterGraphics, Position, StatusEffects, Skills, Hotbar, Destination,
      RidingItems, BulletItems, Inventory, Party, Wishlist, Quests,
      RoseCommon::EntityAPI>;

using Entity = EntityManager::Entity;

#include "partybase.h"

uint16_t getId(Entity entity);

std::string &getName(Entity entity);

std::shared_ptr<CMapClient> getClient(Entity entity);

bool isOnMap(Entity entity);

#endif /* !_ENTITYCOMPONENTS_H_ */
