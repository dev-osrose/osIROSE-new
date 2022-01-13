#include "mouse/mouse_cmd.h"

#include "logconsole.h"
#include "entity_system.h"

#include "srv_mouse_cmd.h"

#include "components/basic_info.h"
#include "components/destination.h"
#include "components/position.h"
#include "components/target.h"

#include <cmath>

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Mouse::mouse_cmd(EntitySystem& entitySystem, Entity entity, const CliMouseCmd& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Mouse::mouse_cmd");
    logger->trace("entity {}, target {} x {} y {}", entity, packet.get_targetId(), packet.get_x(), packet.get_y());

    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto& pos = entitySystem.get_component<Component::Position>(entity);
    auto& dest = entitySystem.add_or_replace_component<Component::Destination>(entity);

    dest.x = packet.get_x();
    dest.y = packet.get_y();
    dest.z = 0;

    const float dx = pos.x - dest.x;
    const float dy = pos.y - dest.y;
    dest.dist = std::sqrt(dx * dx + dy * dy);
    
    if (packet.get_targetId()) {
        Entity t = entitySystem.get_entity_from_id(packet.get_targetId());
        if (t != entt::null) {
            auto& target = entitySystem.add_or_replace_component<Component::Target>(entity);
            target.target = t;
        }
    } else {
        entitySystem.remove_component<Component::Target>(entity);
    }

    auto p = SrvMouseCmd::create(basicInfo.id);
    p.set_targetId(packet.get_targetId());
    p.set_x(packet.get_x());
    p.set_y(packet.get_y());
    entitySystem.send_nearby(entity, p);
}

void Mouse::stop_moving(EntitySystem& entitySystem, Entity entity, const CliStopMoving& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Mouse::stop_moving");
    logger->trace("entity {}, x {} y {} z {}", entity, packet.get_x(), packet.get_y(), packet.get_z());

    entitySystem.remove_component<Component::Destination>(entity);

    entitySystem.update_position(entity, packet.get_x(), packet.get_y());
}
