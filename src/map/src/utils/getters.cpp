#include "getters.h"

int  Utils::get_attack_speed(EntitySystem& entitySystem, RoseCommon::Entity entity) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  if(entity) {
    if(auto* lua = entitySystem.try_get_component<Component::ItemLua>(entity); lua) {
      if(auto api = lua->api.lock(); api)
        return api->get_attack_speed();
      } else {
        logger->warn("Utils::get_attack_speed(): No lua api available for {}", entity);
      }
    } else {
      logger->warn("Utils::get_attack_speed(): No lua component for {}", entity);
    }
  } else {
    logger->warn("Utils::get_attack_speed(): Not a valid entity {}", entity);
  }
  return 0;
}

int  Utils::get_move_speed(EntitySystem& entitySystem, RoseCommon::Entity entity) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  if(entity) {
    if(auto* lua = entitySystem.try_get_component<Component::ItemLua>(entity); lua) {
      if(auto api = lua->api.lock(); api)
        return api->get_move_speed();
      } else {
        logger->warn("Utils::get_move_speed(): No lua api available for {}", entity);
      }
    } else {
      logger->warn("Utils::get_move_speed(): No lua component for {}", entity);
    }
  } else {
    logger->warn("Utils::get_move_speed(): Not a valid entity {}", entity);
  }
  return 0;
}

int  Utils::get_magic(EntitySystem& entitySystem, RoseCommon::Entity entity) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  if(entity) {
    if(auto* lua = entitySystem.try_get_component<Component::ItemLua>(entity); lua) {
      if(auto api = lua->api.lock(); api)
        return api->get_magic();
      } else {
        logger->warn("Utils::get_magic(): No lua api available for {}", entity);
      }
    } else {
      logger->warn("Utils::get_magic(): No lua component for {}", entity);
    }
  } else {
    logger->warn("Utils::get_magic(): Not a valid entity {}", entity);
  }
  return 0;
}
