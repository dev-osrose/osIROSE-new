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
 * \file entitysystem
 * \brief The world model that holds onto the systems and entities
 *
 * \author L3nn0x
 * \date october 2016
 */
#ifndef _ENTITYSYSTEM_H_
#define _ENTITYSYSTEM_H_

#include <functional>
#include <mutex>
#include <queue>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "crosepacket.h"
#include "item.h"
#include "systems/system.h"
#include "id_manager.h"

// FIXME : set those values in the config file/database ?
#define NEARBY_DIST 10000  // in game units, how far is considered 'near' // FIXME : make it entity dependent?
#define POSITION_CHEATING 1000 * 1000

/*!
 * \class EntitySystem
 * \brief The world model that contains the systems and the entities
 *
 * \author L3nn0x
 * \date october 2016
 */
class EntitySystem {
 public:
  EntitySystem();

  void update(double dt);

  void destroy(Entity entity);

  Entity create();

  Entity buildItemEntity(Entity creator, RoseCommon::Item&& item);

  template <typename T>
  T& get() {
    return *systemManager_.get<T>();
  }

  bool dispatch(Entity entity, std::unique_ptr<RoseCommon::CRosePacket> packet);

  Entity loadCharacter(uint32_t charId, bool platinium);
  void saveCharacter(uint32_t charId, Entity entity);

  bool isNearby(Entity a, Entity b);

  void registerEntity(Entity entity);

  Entity getEntity(const std::string& name);
  Entity getEntity(uint32_t charId);

  Entity getItemEntity(uint32_t id);

  EntityManager& getEntityManager();
 
  Entity create_warpgate(std::string alias, int dest_map_id, float dest_x, float dest_y, float dest_z,
                      int map_id, float x, float y, float z, float angle,
                      float x_scale, float y_scale, float z_scale);
  Entity create_npc(std::string npc_lua, int npc_id, int map_id, float x, float y, float z, float angle);
  Entity create_spawner(std::string alias, int mob_id, int mob_count,
                     int spawner_limit, int spawner_interval, int spawner_range,
                     int map_id, float x, float y, float z);

  void bulk_destroy(const std::vector<Entity>& s);

 private:
  EntityManager entityManager_;
  SystemManager systemManager_;
  mutable std::mutex access_;
  std::vector<Entity> toDestroy_;
  std::unordered_map<std::string, Entity> nameToEntity_;
  std::unordered_map<uint32_t, Entity> idToEntity_;
  std::queue<std::pair<Entity, std::unique_ptr<RoseCommon::CRosePacket>>> toDispatch_;
  uint32_t nextId_;
  std::unordered_map<uint32_t, Entity> itemToEntity_;
  IdManager id_manager_;
};

#endif /* !_ENTITYSYSTEM_H_ */
