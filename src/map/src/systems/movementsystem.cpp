#include <cmath>
#include <algorithm>

#include "systems/movementsystem.h"
#include "cli_mousecmd.h"
#include "cli_stopmoving.h"
#include "cmapserver.h"
#include "packetfactory.h"
#include "srv_mousecmd.h"
#include "srv_stopmoving.h"
#include "srv_adjustposition.h"
#include "srv_switchserver.h"
#include "config.h"
#include "cmapclient.h"

using namespace Systems;
using namespace RoseCommon;

MovementSystem::MovementSystem(SystemManager &manager) : System(manager) {
  manager.registerDispatcher(ePacketType::PAKCS_MOUSE_CMD, &MovementSystem::processMove);
  manager.registerDispatcher(ePacketType::PAKCS_STOP_MOVING, &MovementSystem::stopMoving);
  manager.getEntityManager().on_component_removed<Destination>([this](Entity entity, Destination *dest) {
    if (!entity) return;
    if (auto client = getClient(entity))
      manager_.SendPacket(client, CMapServer::eSendType::NEARBY,
                                             *makePacket<ePacketType::PAKWC_STOP_MOVING>(entity));
    // TODO: check what type entity.component<BasicInfo>()->targetId_ is and execute corresponding action (npc talk, attack, item pickup...)
  });
  manager.getEntityManager().on_component_added<Position>([this](Entity entity, Position*) {
      updatePosition(entity);
  });
}

void MovementSystem::update(EntityManager &es, double dt) {
  for (Entity entity : es.entities_with_components<Position, Destination, AdvancedInfo>()) {
    Position *position = entity.component<Position>();
    Destination *destination = entity.component<Destination>();
    AdvancedInfo *advanced = entity.component<AdvancedInfo>();
    float dx = destination->x_ - position->x_;
    float dy = destination->y_ - position->y_;
    float distance = std::sqrt(dx * dx + dy * dy);
    destination->dist_ = distance;
    float speed = advanced->runSpeed_;
    float ntime = distance / speed;
    float old_x = position->x_;
    float old_y = position->y_;
    if (ntime <= dt || distance == 0) {
      position->x_ = destination->x_;
      position->y_ = destination->y_;
      entity.remove<Destination>();
    } else {
      position->x_ += dx * dt / ntime;
      position->y_ += dy * dt / ntime;
    }
    updatePosition(entity, old_x, old_y);
    if (Entity e = is_on_warpgate(entity); e) {
        auto dest = e.component<Destination>();
        teleport(entity, dest->dist_, dest->x_, dest->y_);
    }
  }
}

void MovementSystem::move(Entity entity, float x, float y, uint16_t target) {
  if (!entity) return;
  auto dest = entity.component<Destination>();
  auto pos = entity.component<Position>();
  float dx = pos->x_ - x;
  float dy = pos->y_ - y;
  float dist = std::sqrt(dx * dx + dy * dy);
  if (dest) {
    dest->x_ = x;
    dest->y_ = y;
    dest->dist_ = dist;
  } else {
    entity.assign<Destination>(x, y, dist);
  }
  entity.component<BasicInfo>()->targetId_ = target;
  // FIXME: what happens if the entity is an NPC or a monster?
  if (auto client = getClient(entity))
    manager_.SendPacket(client, CMapServer::eSendType::EVERYONE, *makePacket<ePacketType::PAKWC_MOUSE_CMD>(entity));
}

void MovementSystem::stop(Entity entity, float x, float y) {
  if (!entity || !entity.component<BasicInfo>()) return;
  entity.remove<Destination>();
  auto position = entity.component<Position>();
  if (position) {
    float dx = position->x_ - x;
    float dy = position->y_ - y;
    if (dx * dx + dy * dy < POSITION_CHEATING) {
      position->x_ = x;
      position->y_ = y;
    } else
      logger_->warn("Player {} attempted to cheat his position : calculated position : ({}, {}), got ({}, {})",
                    getId(entity), position->x_, position->y_, x, y);
  }
}

void MovementSystem::processMove(CMapClient &, Entity entity, const CliMouseCmd &packet) {
  logger_->trace("MovementSystem::processMove");
  if (!entity.component<Position>() || !entity.component<BasicInfo>()) return;
  move(entity, packet.x(), packet.y(), packet.targetId());
}

void MovementSystem::stopMoving(CMapClient &, Entity entity, const CliStopMoving &packet) {
  logger_->trace("MovementSystem::stopMoving");
  stop(entity, packet.x(), packet.y());
}

void MovementSystem::teleport(Entity entity, uint16_t map_id, float x, float y) {
    logger_->trace("MovementSystem::teleport");
    logger_->debug("Teleporting {} to {} {} {}", getId(entity), map_id, x, y);
    entity.component<BasicInfo>()->targetId_ = 0; // to avoid doing anything in the destination callback
    entity.remove<Destination>();
    auto pos = entity.component<Position>();
    pos->x_ = x;
    pos->y_ = y;
    if (pos->map_ == map_id) {
        manager_.SendPacket(getClient(entity), CMapServer::eSendType::EVERYONE, 
                            *makePacket<ePacketType::PAKWC_ADJUST_POSITION>(entity));
    } else {
        if (auto client = getClient(entity)) {
            auto &config = Core::Config::getInstance();
            client->switch_server();
            client->send(*makePacket<ePacketType::PAKCC_SWITCH_SERVER>(
                config.mapServer().clientPort + map_id,
                client->get_session_id(),
                0,
                config.serverData().ip));
            client->disconnect();
        }
    }
}

namespace {
constexpr std::tuple<uint16_t, uint16_t> get_grid_position(float x, float y) {
    uint16_t gx = x / 1000.f;
    uint16_t gy = y / 1000.f;
    return {gx, gy};
}

std::tuple<uint16_t, uint16_t> get_grid_position(Entity e) {
    auto pos = e.component<Position>();
    if (!pos) return {0, 0};
    return get_grid_position(pos->x_, pos->y_);
}
}

bool MovementSystem::is_nearby(Entity a, Entity b) const {
    if (!a || !b) return false;
    auto pos_a = get_grid_position(a);
    auto pos_b = get_grid_position(b);
    if (std::abs(std::get<0>(pos_a) - std::get<0>(pos_b)) <= 1
        && std::abs(std::get<1>(pos_a) - std::get<1>(pos_b)) <= 1)
        return true;
    return false;
}

void MovementSystem::updatePosition(Entity e, float old_x, float old_y) {
    if (old_x && old_y) {
        auto &list = grid[get_grid_position(old_x, old_y)];
        std::remove(list.begin(), list.end(), e);
    }
    grid[get_grid_position(e)].push_back(e);
}

Entity MovementSystem::is_on_warpgate(Entity e) {
    auto pos = e.component<Position>();
    for (auto &it : grid[get_grid_position(e)]) {
        if (it.component<Warpgate>()) continue; // skip non warpgates
        auto warp_pos = it.component<Position>();
        float dx = pos->x_ - warp_pos->x_;
        float dy = pos->x_ - warp_pos->y_;
        // FIXME: this should actually be a matrix multiplication to compute the warpgate final position
        // rot * scale * pos
        if (dx * dx + dy * dy < WARPGATE_DISTANCE) return it;
    }
    return {};
}
