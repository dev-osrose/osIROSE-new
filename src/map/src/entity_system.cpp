#include "entity_system.h"
#include <entt.hpp>
#include "components/basic_info.h"
#include "components/computed_values.h"
#include "components/faction.h"
#include "components/graphics.h"
#include "components/guild.h"
#include "components/hotbar.h"
#include "components/inventory.h"
#include "components/level.h"
#include "components/life.h"
#include "components/magic.h"
#include "components/npc.h"
#include "components/owner.h"
#include "components/position.h"
#include "components/skills.h"
#include "components/stamina.h"
#include "components/stats.h"
#include "components/status_effects.h"
#include "components/wishlist.h"

EntitySystem::EntitySystem(std::chrono::milliseconds maxTimePerUpdate) : maxTimePerUpdate(maxTimePerUpdate) {
	logger = Core::CLog::GetLogger(Core::log_type::NETWORK).lock();
}

EntitySystem::~EntitySystem() {
    work_queue.kill();
}

void EntitySystem::add_task(std::function<void(RoseCommon::Registry&, std::chrono::milliseconds)>&& task) {
    work_queue.push_back(std::move(task));
}

void EntitySystem::update(std::chrono::milliseconds dt) {
    auto start = Core::Time::GetTickCount();
    for (auto [res, task] = work_queue.pop_front(); res;) {
        std::lock_guard<std::mutex> lock(access);
        task(registry, dt);
        const std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(Core::Time::GetTickCount() - start);
        if (diff >= maxTimePerUpdate) {
            logger->warn("Stopping after {}ms, {} tasks remaining", maxTimePerUpdate.count(), work_queue.size());
            break;
        }
        auto [tmp_res, tmp_task] = work_queue.pop_front();
        res = tmp_res;
        task = std::move(tmp_task);
    }
}

RoseCommon::Entity EntitySystem::load_character(uint32_t charId, bool platinium) {
    using namespace Component;
    auto conn = Core::connectionPool.getConnection(Core::osirose);
    Core::CharacterTable characters{};
    Core::InventoryTable inventoryTable{};
    Core::SkillTable skillsTable{};

    auto charRes = conn(sqlpp::select(sqlpp::count(characters.id), sqlpp::all_of(characters))
                          .from(characters).where(characters.id == charId));

    if (static_cast<long>(charRes.front().count) != 1L) {
        return entt::null;
    }
    const auto& charRow = charRes.front();

    entt::prototype prototype(registry);

    auto& basicInfo = prototype.set<BasicInfo>();
    basicInfo.name = charRow.name;
    basicInfo.id = idManager.get_free_id();
    basicInfo.tag = basicInfo.id;
    basicInfo.teamId = basicInfo.id;
    basicInfo.job = charRow.job;
    basicInfo.statPoints = charRow.stat_points;
    basicInfo.skillPoints = charRow.skill_points;
    basicInfo.pkFlag = charRow.pk_flag;

    auto& computedValues = prototype.set<ComputedValues>();
    computedValues.command = RoseCommon::Command::STOP;
    computedValues.isOnMap.store(false);
    computedValues.moveMode = RoseCommon::MoveMode::WALK;
    computedValues.runSpeed = 0;
    computedValues.atkSpeed = 0;
    computedValues.weightRate = 0;
    
    auto& faction = prototype.set<Faction>();
    faction.id = charRow.factionid;
    faction.rank = charRow.faction_rank;
    faction.fame = charRow.fame;
    faction.factionFame[0] = charRow.faction_fame1;
    faction.factionFame[1] = charRow.faction_fame2;
    faction.points[0] = charRow.faction_points1;
    faction.points[1] = charRow.faction_points2;
    faction.points[2] = charRow.faction_points3;

    auto& graphics = prototype.set<Graphics>();
    graphics.face = charRow.face;
    graphics.hair = charRow.hair;
    graphics.race = charRow.race;
    
    auto& guild = prototype.set<Guild>();
    guild.id = charRow.clanid;
    guild.contribution = charRow.clan_contribution;
    guild.rank = charRow.clan_rank;

    prototype.set<Hotbar>();

    prototype.set<Inventory>();

    auto& level = prototype.set<Level>();
    level.xp = charRow.exp;
    level.level = charRow.level;
    level.penaltyXp = charRow.penalty_exp;

    auto& life = prototype.set<Life>();
    life.hp = charRow.maxHp / 3; // you only get 30% of your health when login in
    life.maxHp = charRow.maxHp;

    auto& magic = prototype.set<Magic>();
    magic.mp = charRow.maxMp / 3;
    magic.maxMp = charRow.maxMp;

    auto& pos = prototype.set<Position>();
    pos.x = charRow.x;
    pos.y = charRow.y;
    pos.z = 0;
    pos.spawn = charRow.reviveMap;

    prototype.set<Skills>();

    auto& stamina = prototype.set<Stamina>();
    stamina.stamina = charRow.stamina;

    auto& stats = prototype.set<Stats>();
    stats.str = charRow.str;
    stats.dex = charRow.dex;
    stats.int_ = charRow.int_;
    stats.con = charRow.con;
    stats.charm = charRow.charm;
    stats.sense = charRow.sense;
    stats.bodySize = 100;
    stats.headSize = 100;

    prototype.set<StatusEffects>();

    prototype.set<Wishlist>();

    std::lock_guard<std::mutex> lock(access);
    return prototype();
}

