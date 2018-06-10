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

std::vector<std::string> ScriptLoader::get_fuzzy_files(const std::string& pattern, size_t max_res) {
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
        
        logger_->info("(Re)loading scripts from '{}'", path);
        state_.script_file(path, env);
        logger_->info("Loaded {} warpgates", warpgates.size());
        logger_->info("Loaded {} npcs", npcs.size());
        logger_->info("Loaded {} spawners", spawners.size());
        logger_->info("Finished (re)loading scripts from '{}'", path);

        if (warpgates.size()) warpgate_files_.insert_or_assign(path, std::move(warpgates));
        if (npcs.size()) npc_files_.insert_or_assign(path, std::move(npcs));
        if (spawners.size()) spawner_files_.insert_or_assign(math, std::move(spawners));

        if (warpgates.size() || npcs.size() || spawners.size()) {
            for (auto& f : files_) if (f == path) return;
            files_.push_back(path);
        }
    } catch (const sol::error& e) {
        logger_->error("Error (re)loading lua scripts '{}' : {}", path, e.what());
    }
}
