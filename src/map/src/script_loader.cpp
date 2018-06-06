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

namespace {

template <typename Func, typename Args...>
constexpr void caller(std::vector<Entity>& entities, EntitySystem& entity_system, Args&&... args) {
    entities.push_back(entity_system.*Func(args...);
}

template <typename Func, typename Tuple, std::size_t... Is>
constexpr void factory_helper(sol::environment& env, std::string const& name, std::vector<Entity>& e, EntitySystem& s, Tuple tuple, std::index_sequence<Is...>) {
    env.set_function(name, caller<Func, decltype(std::get<Is>(tuple))...>, e, s); 
}

template <typename Func, typename... Args>
constexpr void factory(sol::environment& env, std::string const& name, std::vector<Entity>& e, EntitySystem& s, std::tuple<Args...> tuple) {
    factory_helper<Func>(env, name, e, s, tuple, std::index_sequence_for<Args...>{});

}

void ScriptLoader::load_script(std::string const& path) {
    try {
        sol::environment env{state_, sol::create, state_.globals()};
        
        auto warpgate_file = warpgate_files_.insert(File{path}, {});
        factory<EntitySystem::create_warpgate>(env, "warp_gate", warpgate_file->second, *entity_system_, ScriptLoader::warpgate_args{});
        
        auto npc_file = npc_files_.insert(File{path}, {});
        factory<EntitySystem::create_npc>(env, "npc", npc_file->second, *entity_system_, ScriptLoader::npc_args{});
  
        auto spawner_file = spawner_files_.insert(File{path}, {});
        factory<EntitySystem::create_spawner>(env, "spawner", spawner_file->second, *entity_system_, ScriptLoader::spawner_args{});
        
        state_.script(path, env);
        logger_->info("Finished (re)loading scripts from '{}'", path);
    } catch (const sol::error& e) {
        logger_->error("Error (re)loading lua scripts '{}' : {}", path, e.what());
    }
}
