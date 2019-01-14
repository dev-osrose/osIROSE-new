#include "script_loader.h"
#include "throwassert.h"
#include "fuzzy_matching.h"

using namespace LuaScript;

ScriptLoader::ScriptLoader(std::shared_ptr<EntitySystem> entity_system, uint16_t map_id, std::string const& path):
entity_system_(entity_system),
logger_(Core::CLog::GetLogger(Core::log_type::SCRIPTLOADER).lock()),
path_(path), map_id_(map_id) {
    state_.open_libraries(); //FIXME: check if we need all libs

    std::size_t pos = path.find_last_of('/');
    throw_assert(pos != std::string::npos, "Error, default path '" << path << "' is not valid");
    std::string base = path.substr(0, pos + 1);

    state_.set_function("include", [this, base](std::string path) {
        ScriptLoader::load_script(base + path);
    });
}

std::vector<std::string> ScriptLoader::get_fuzzy_files(const std::string& pattern, size_t max_res) const {
    std::vector<Core::FuzzyMatch> res = Core::fuzzy_match(files_, pattern);

    std::vector<std::string> ret{max_res};

    size_t i = 0;
    while (max_res--) {
        ret.push_back(files_[res[i++].index]);
    }
    return ret;
}

void ScriptLoader::load_script() {
    load_script(path_);
}

void ScriptLoader::load_npcs() {
    for (auto& [file, entities] : npc_files_) {
        entity_system_->bulk_destroy(entities);
        entities.clear();
        load_script(file);
    }
}

void ScriptLoader::load_warpgates() {
    for (auto& [file, entities] : warpgate_files_) {
        entity_system_->bulk_destroy(entities);
        entities.clear();
        load_script(file);
    }
}

void ScriptLoader::load_spawners() {
    for (auto& [file, entities] : spawner_files_) {
        entity_system_->bulk_destroy(entities);
        entities.clear();
        load_script(file);
    }
}

void ScriptLoader::load_script(std::string const& path) {
    try {
        sol::environment env{state_, sol::create, state_.globals()};
        
        auto warpgate_file = warpgate_files_.find(path);
        std::vector<Entity> warpgates;
        if (warpgate_file != warpgate_files_.end()) {
            warpgates = std::move(warpgate_file->second);
        }
        env.set_function("warp_gate", [&warpgates, this](std::string alias, int dest_map_id, float dest_x, float dest_y, float dest_z, int map_id, float x, float y, float z, float angle, float x_scale, float y_scale, float z_scale) {
            if (map_id != map_id_) return;
            warpgates.push_back(entity_system_->create_warpgate(alias, dest_map_id, dest_x, dest_y, dest_z, map_id, x, y, z, angle, x_scale, y_scale, z_scale));
        });
        
        auto npc_file = npc_files_.find(path);
        std::vector<Entity> npcs;
        if (npc_file != npc_files_.end()) {
            npcs = std::move(npc_file->second);
        }
        env.set_function("npc", [&npcs, this](std::string npc_lua, int npc_id, int map_id, float x, float y, float z, float angle) {
            if (map_id != map_id_) return;
            npcs.push_back(entity_system_->create_npc(npc_lua, npc_id, map_id, x, y, z, angle));
        });
        
        auto spawner_file = spawner_files_.find(path);
        std::vector<Entity> spawners;
        if (spawner_file != spawner_files_.end()) {
            spawners = std::move(spawner_file->second);
        }
        env.set_function("mob", [&spawners, this](std::string alias, int mob_id, int mob_count, int limit, int interval, int range, int map_id, float x, float y, float z) {
            if (map_id != map_id_) return;
            spawners.push_back(entity_system_->create_spawner(alias, mob_id, mob_count, limit, interval, range, map_id, x, y, z));
        });
        
        auto spawn_file = player_spawn_files_.find(path);
        std::vector<Entity> player_spawns;
        if (spawn_file != player_spawn_files_.end()) {
            player_spawns = std::move(spawn_file->second);
        }
        
        env.set_function("revive_point", [&player_spawns, this](int map_id, float dest_x, float dest_y) {
            // Combat system needs all map's login spawn points to revive the player at a saved location
            player_spawns.push_back(entity_system_->create_player_spawn(PlayerSpawn::REVIVE_POINT, map_id, dest_x, dest_y));
        });
        env.set_function("start_point", [&player_spawns, this](int map_id, float dest_x, float dest_y) {
            // Combat system needs all map's login spawn points to revive the player at a saved location
            player_spawns.push_back(entity_system_->create_player_spawn(PlayerSpawn::START_POINT, map_id, dest_x, dest_y));
        });
        env.set_function("respawn_point", [&player_spawns, this](int map_id, float dest_x, float dest_y) {
            if (map_id != map_id_) return;
            player_spawns.push_back(entity_system_->create_player_spawn(PlayerSpawn::RESPAWN_POINT, map_id, dest_x, dest_y));
        });
        
        auto ai_file = ai_files_.find(path);
        std::vector<Entity> ai_logic;
        if (ai_file != ai_files_.end()) {
            ai_logic = std::move(ai_file->second);
        }
        env.set_function("registerNpc", [&ai_logic, this](int npc_id, sol::table npc_data) {
          // Battle data
          auto walk_speed = npc_data.get<sol::optional<int>>("walk_speed");
          auto run_speed = npc_data.get<sol::optional<int>>("run_speed");
          auto right_weapon = npc_data.get<sol::optional<int>>("r_weapon");
          auto left_weapon = npc_data.get<sol::optional<int>>("l_weapon");
          auto level = npc_data.get<sol::optional<int>>("level");
          auto hp = npc_data.get<sol::optional<int>>("hp");
          auto attack = npc_data.get<sol::optional<int>>("attack");
          auto hit = npc_data.get<sol::optional<int>>("hit");
          auto def = npc_data.get<sol::optional<int>>("def");
          auto res = npc_data.get<sol::optional<int>>("res");
          auto avoid = npc_data.get<sol::optional<int>>("avoid");
          auto attack_speed = npc_data.get<sol::optional<int>>("attack_spd");
          auto is_magic_damage = npc_data.get<sol::optional<bool>>("is_magic_damage");
          auto give_exp = npc_data.get<sol::optional<int>>("give_exp");
          auto drop_type = npc_data.get<sol::optional<int>>("drop_type");
          auto drop_money = npc_data.get<sol::optional<int>>("drop_money");
          auto drop_item = npc_data.get<sol::optional<int>>("drop_item");
          auto union_number = npc_data.get<sol::optional<int>>("union_number");
          auto need_summon_count = npc_data.get<sol::optional<int>>("need_summon_count");
          auto attack_range = npc_data.get<sol::optional<int>>("attack_range");
          auto can_target = npc_data.get<sol::optional<int>>("can_target");
          
          //logic data
          auto summon_mob_type = npc_data.get<sol::optional<int>>("summon_mob_type");
          auto quest_type = npc_data.get<sol::optional<int>>("quest_type");
          auto npc_type = npc_data.get<sol::optional<int>>("npc_type");
          auto ai_type = npc_data.get<sol::optional<int>>("ai_type");
          
          // This is indexes into LIST_SELL.STB
          auto sell_tab0 = npc_data.get<sol::optional<int>>("sell_tab0");
          auto sell_tab1 = npc_data.get<sol::optional<int>>("sell_tab1");
          auto sell_tab2 = npc_data.get<sol::optional<int>>("sell_tab2");
          auto sell_tab3 = npc_data.get<sol::optional<int>>("sell_tab3");
          
          // Misc/graphical data
          auto hit_material_type = npc_data.get<sol::optional<int>>("hit_material_type");
          auto face_icon = npc_data.get<sol::optional<int>>("face_icon");
          auto height = npc_data.get<sol::optional<int>>("height");
          auto scale = npc_data.get<sol::optional<int>>("scale");
          //logger_->info("{} - {} - {} - {} - {} - {}", run_speed.value(), scale.value(), level.value(), hp.value(), attack.value(), hit.value());
          //ai.push_back(entity_system_->create_npc(npc_lua, npc_id, map_id, x, y, z, angle));
        });
        
        logger_->trace("(Re)loading scripts from '{}'", path);
        state_.script_file(path, env);
        if (warpgates.size()) logger_->trace("Map {} loaded {} warpgates", map_id_, warpgates.size());
        if (npcs.size()) logger_->trace("Map {} loaded {} npcs", map_id_, npcs.size());
        if (spawners.size()) logger_->trace("Map {} loaded {} mob spawners", map_id_, spawners.size());
        if (player_spawns.size()) logger_->trace("Map {} loaded {} player_spawns", map_id_, player_spawns.size());
        if (ai_logic.size()) logger_->trace("Map {} loaded {} ai logic files", map_id_, ai_logic.size());
        logger_->trace("Finished (re)loading scripts from '{}'", path);

        if (warpgates.size()) warpgate_files_.insert_or_assign(path, std::move(warpgates));
        if (npcs.size()) npc_files_.insert_or_assign(path, std::move(npcs));
        if (spawners.size()) spawner_files_.insert_or_assign(path, std::move(spawners));
        if (player_spawns.size()) player_spawn_files_.insert_or_assign(path, std::move(player_spawns));
        if (ai_logic.size()) ai_files_.insert_or_assign(path, std::move(ai_logic));

        if (warpgates.size() || npcs.size() || spawners.size() || player_spawns.size() || ai_logic.size()) {
            for (auto& f : files_) if (f == path) return;
            files_.push_back(path);
        }
    } catch (const sol::error& e) {
        logger_->error("Error (re)loading lua scripts '{}' : {}", path, e.what());
    }
}
