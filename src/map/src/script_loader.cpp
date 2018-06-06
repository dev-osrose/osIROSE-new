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

    state_.set_function("include", this->reload_scripts);
    state_.set_function("warp_gate", entity_system->create_warpgate);
    state_.set_function("npc", entity_system->create_npc);
    state_.set_function("mob", entity_system->create_spawner);
}

void ScriptLoader::reload_scripts() {
    reload_scripts(path_);
}

void ScriptLoader::reload_npcs() {
    for (auto& file : npc_files_)
        file.replace_entities(reload_scripts(file.path()));
}

void ScriptLoader::reload_warpgates() {
    for (auto& file : warpgate_files_)
        file.replace_entities(reload_scripts(file.path()));
}

void ScriptLoader::reload_spawners() {
    for (auto& file : spawner_files_)
        file.replace_entities(reload_scripts(file.path()));
}

void ScriptLoader::reload_scripts(std::string const& path) {
    try {
        if (auto it = npc_files_.find(File(path)); it != npc_files_.end())
            entity_system_->bulk_destroy(it->entities());
        if (auto it = warpgate_files_.find(File(path)); it != warpgate_files_.end())
            entity_system_->bulk_destroy(it->entities());
        if (auto it = spawner_files_.find(File(path)); it != spawner_files_.end())
            entity_system_->bulk_destroy(it->entities());
        state_.script(path);
        logger_->info("Finished (re)loading scripts from '{}'", path);
    } catch (const sol::error& e) {
        logger_->error("Error (re)loading lua scripts '{}' : {}", path, e.what());
    }
}
