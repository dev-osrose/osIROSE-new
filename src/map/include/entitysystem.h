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
#include <chrono>
#include "crosepacket.h"
#include "item.h"
#include "systems/system.h"
#include "id_manager.h"

class CMapServer;

namespace LuaScript {
class ScriptLoader;
}

/*!
 * \class EntitySystem
 * \brief The world model that contains the systems and the entities
 *
 * \author L3nn0x
 * \date october 2016
 */
class EntitySystem {
 public:
  EntitySystem(CMapServer *server);

  void update(std::chrono::milliseconds dt);

  void destroy(Entity entity, bool save);

  Entity create();

  Entity buildItemEntity(Entity creator, RoseCommon::Item&& item);

  Entity buildMobEntity(Entity spawner);

  template <typename T>
  T& get() {
    return *systemManager_.get<T>();
  }

  bool dispatch(Entity entity, std::unique_ptr<RoseCommon::CRosePacket> packet);

  Entity loadCharacter(uint32_t charId, bool platinium);
  void saveCharacter(uint32_t charId, Entity entity);

  bool isNearby(Entity a, Entity b);

  void registerEntity(Entity entity);

  void unregisterEntity(Entity entity);

  Entity getEntity(const std::string& name);
  Entity getEntity(uint32_t id);

  EntityManager& getEntityManager();
 
  Entity create_warpgate(std::string alias, int dest_map_id, float dest_x, float dest_y, float dest_z,
                      int map_id, float x, float y, float z, float angle,
                      float x_scale, float y_scale, float z_scale);
  Entity create_npc(std::string npc_lua, int npc_id, int map_id, float x, float y, float z, float angle);
  Entity create_spawner(std::string alias, int mob_id, int mob_count,
                     int spawner_limit, int spawner_interval, int spawner_range,
                     int map_id, float x, float y, float z);

  void bulk_destroy(const std::vector<Entity>& s);
 
  LuaScript::ScriptLoader& get_script_loader() noexcept;
 
 void SendPacket(const std::shared_ptr<CMapClient>& sender, RoseCommon::CRoseServer::eSendType type,
                  RoseCommon::CRosePacket& _buffer);
  void SendPacket(const CMapClient& sender, RoseCommon::CRoseServer::eSendType type,
                  RoseCommon::CRosePacket& _buffer);

 private:
  EntityManager entityManager_;
  SystemManager systemManager_;
  mutable std::mutex access_;
  std::unordered_map<std::string, Entity> nameToEntity_;
  std::unordered_map<uint32_t, Entity> idToEntity_;
  std::queue<std::pair<Entity, std::unique_ptr<RoseCommon::CRosePacket>>> toDispatch_;
  IdManager id_manager_;
  CMapServer *server_;

 private:
  struct CommandBase {
      virtual ~CommandBase() = default;
      virtual void execute(EntitySystem&) = 0;
  };

  template <typename Func>
  struct Command : public CommandBase {
      Command(Func&& f) : func_(std::move(f)) {}
      virtual ~Command() = default;
      virtual void execute(EntitySystem& e) override {
          func_(e);
      }
      private:
        Func func_;
  };

  std::vector<std::unique_ptr<CommandBase>> create_commands_;
  std::vector<std::unique_ptr<CommandBase>> delete_commands_;

 private:
  static constexpr double drop_radius = 50.f;
};

#endif /* !_ENTITYSYSTEM_H_ */
