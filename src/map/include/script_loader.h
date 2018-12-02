#pragma once

#include "entitysystem.h"
#include "logconsole.h"

#include <sol.hpp>

#include <unordered_map>
#include <vector>

namespace LuaScript {

class ScriptLoader {
    public:
        ScriptLoader(std::shared_ptr<EntitySystem> entity_system, uint16_t map_id, std::string const& path);
        ScriptLoader(const ScriptLoader&) = delete;
        ScriptLoader(ScriptLoader&&) = delete;
        ScriptLoader& operator=(const ScriptLoader&) = delete;

        void load_script();
        void load_script(std::string const& path);
        void load_npcs();
        void load_warpgates();
        void load_spawners();

        std::vector<std::string> get_fuzzy_files(const std::string& pattern, size_t max_res = 3) const;

    private:
        std::shared_ptr<EntitySystem> entity_system_;
        std::shared_ptr<spdlog::logger> logger_;
        std::string path_;
        sol::state state_;
        uint16_t map_id_;

        std::vector<std::string> files_;

        std::unordered_map<std::string, std::vector<Entity>> ai_files_;
        std::unordered_map<std::string, std::vector<Entity>> npc_files_;
        std::unordered_map<std::string, std::vector<Entity>> warpgate_files_;
        std::unordered_map<std::string, std::vector<Entity>> spawner_files_;
        std::unordered_map<std::string, std::vector<Entity>> player_spawn_files_;
};

}
