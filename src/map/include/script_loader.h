#pragma once

#include "entitysystem.h"
#include "logconsole.h"

#include <sol.hpp>

#include <set>

namespace LuaScript {

class ScriptLoader {
    class File {
        public:
            File(std::string const& path);

            bool has_filename(std::string const& filename) const noexcept {
                return filename_ == filename;
            }

            bool operator==(File const& other) const noexcept {
                return path_ == other.path_;
            }

            const std::string& path() const noexcept { return path_; }

            void replace_entities(std::set<Entity> const& s) {
                entities_ = s;
            }

            const std::set<Entity>& entities() const noexcept {
                return entities_;
            }

        private:
            std::string filename_;
            std::string path_;
            std::set<Entity> entities_;
    };

    public:
        ScriptLoader(std::shared_ptr<EntitySystem> entity_system,uint16_t map_id, std::string const& path);

        void reload_scripts();
        void reload_scripts(std::string const& path);
        void reload_npcs();
        void reload_warpgates();
        void reload_spawners();

    private:
        std::shared_ptr<EntitySystem> entity_system_;
        std::shared_ptr<spdlog::logger> logger_;
        std::string path_;
        sol::state state_;
        uint16_t map_id_;

        std::set<File> npc_files_;
        std::set<File> warpgate_files_;
        std::set<File> spawner_files_;
};

}
