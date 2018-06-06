#pragma once

#include "entitysystem.h"
#include "logconsole.h"

#include <sol.hpp>

#include <set>

namespace LuaScript {

class ScriptLoader {
    struct File {
        File() = default;
        File(std::string const& path);
        
        std::string filename_;
        std::string path_;
    };

    public:
        ScriptLoader(std::shared_ptr<EntitySystem> entity_system, uint16_t map_id, std::string const& path);

        void load_script();
        void load_script(std::string const& path);
        void load_npcs();
        void load_warpgates();
        void load_spawners();
    
        using warpgate_args = std::tuple<std::string, int, float, float, float, int, float, float, float, float, float, float, float>;
        using npc_args = std::tuple<std::string, int, int, float, float, float, float>;
        using spawner_args = std::tuple<std::stringm int, int, int, int, int, int, float, float, float>;

    private:
        std::shared_ptr<EntitySystem> entity_system_;
        std::shared_ptr<spdlog::logger> logger_;
        std::string path_;
        sol::state state_;
        uint16_t map_id_;

        std::unordered_map<File, std::vector<Entity>> npc_files_;
        std::unordered_map<File, std::vector<Entity>> warpgate_files_;
        std::unordered_map<File, std::vector<Entity>> spawner_files_;
};

}
