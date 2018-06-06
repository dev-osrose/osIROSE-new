#include "script_loader.h"

using namespace LuaScript;

ScriptLoader::File::File(std::string const& path) : path_(path) {
    //TODO: find filename
    //prob split on '/' and take latest, then split on '.' and take everything but the last one if there's more than one
}

ScriptLoader::ScriptLoader(std::shared_ptr<EntitySystem> entity_system, uint16_t map_id, std::string const& path):
entity_system_(entity_system),
logger_(Core::CLog::GetLogger(Core::log_type::SCRIPTLOADER).lock()),
path_(path), map_id_(map_id) {
    state_.open_libraries(); //FIXME: check if we need all libs

    state_.set_function("include", [](std::string path) {
        load_script(path);
    });
}

void ScriptLoader::reload_scripts() {
    reload_scripts(path_);
}

void ScriptLoader::reload_npcs() {
    for (auto& [file, entities] : npc_files_) {
        entity_system_->bulk_destroy(entities);
        entities.clear();
        load_script(file.path_);
    }
}

void ScriptLoader::reload_warpgates() {
    for (auto& [file, entities] : warpgate_files_) {
        entity_system_->bulk_destroy(entities);
        entities.clear();
        load_script(file.path_);
    }
}

void ScriptLoader::load_spawners() {
    for (auto& [file, entities] : spawner_files_) {
        entity_system_->bulk_destroy(entities);
        entities.clear();
        load_script(file.path_);
    }
}

void ScriptLoader::load_script(std::string const& path) {
    try {
        sol::environment env{state_, sol::create, state_.globals()};
        
        auto warpgate_file = warpgate_files_.insert(File{path}, {});
        env.set_function("warp_gate", [warpgate_file]() {
            wargate_file->second.push_back(entity_system_->create_warpgate());
        });
        
        auto npc_file = npc_files_.insert(File{path}, {});
        env.set_function("npc", [npc_file]() {
            npc_file->second.push_back(entity_system_->create_npc());
        });
        
        auto spawner_file = spawner_files_.insert(File{path}, {});
        env.set_function("mob", [spawner_file]() {
            spawner_file->second.push_back(entity_system->create_spawner());
        });
        
        state_.script(path, env);
        logger_->info("Finished (re)loading scripts from '{}'", path);
    } catch (const sol::error& e) {
        logger_->error("Error (re)loading lua scripts '{}' : {}", path, e.what());
    }
}
