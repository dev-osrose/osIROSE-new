#pragma once

#include "entitysystem.h"
#include "logconsole.h"

#include <sol.hpp>

#include <unordered_map>
#include <vector>

namespace LuaScript {

class ScriptLoader {
    struct File {
        File() = default;
        File(std::string const& path);
        
        bool operator==(File const& other) const noexcept {
            return path_ == other.path_;
        }
        
        std::string filename_;
        std::string path_;
    };
    
    struct hash {
        size_t operator()(const File& x) const {
            return std::hash<std::string>()(x.path_);
        }
    };

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

    private:
        std::shared_ptr<EntitySystem> entity_system_;
        std::shared_ptr<spdlog::logger> logger_;
        std::string path_;
        sol::state state_;
        uint16_t map_id_;

        std::unordered_map<File, std::vector<Entity>, hash> npc_files_;
        std::unordered_map<File, std::vector<Entity>, hash> warpgate_files_;
        std::unordered_map<File, std::vector<Entity>, hash> spawner_files_;
};

}
