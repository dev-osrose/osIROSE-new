#include "utils/getters.h"
#include "components/lua.h"

int Utils::get_attack_speed(EntitySystem& entitySystem, Entity item) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  if (item != entt::null) {
    if (auto* lua = entitySystem.try_get_component<Component::ItemLua>(item); lua) {
      if (auto api = lua->api.lock(); api) {
        return api->get_attack_speed();
      } else {
        logger->warn("Utils::get_attack_speed(): No lua api available for {}", item);
      }
    } else {
      logger->warn("Utils::get_attack_speed(): No lua component for {}", item);
    }
  }
  return 0;
}

int Utils::get_move_speed(EntitySystem& entitySystem, Entity item) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  if (item != entt::null) {
    if (auto* lua = entitySystem.try_get_component<Component::ItemLua>(item); lua) {
      if (auto api = lua->api.lock(); api) {
        return api->get_move_speed();
      } else {
        logger->warn("Utils::get_move_speed(): No lua api available for {}", item);
      }
    } else {
      logger->warn("Utils::get_move_speed(): No lua component for {}", item);
    }
  }
  return 0;
}

int Utils::get_magic(EntitySystem& entitySystem, Entity item) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  if (item != entt::null) {
    if (auto* lua = entitySystem.try_get_component<Component::ItemLua>(item); lua) {
      if (auto api = lua->api.lock(); api) {
        return api->get_magic();
      } else {
        logger->warn("Utils::get_magic(): No lua api available for {}", item);
      }
    } else {
      logger->warn("Utils::get_magic(): No lua component for {}", item);
    }
  }
  return 0;
}
