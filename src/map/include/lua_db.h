#pragma once

#include <sol.hpp>
#include <string>
#include <optional>
#include <unordered_map>
#include <unordered_multimap>

class LuaDb {
    public:
        void store_lua(const std::string& filename, sol::environment env, int npc_id, sol::table npc_data);
        std::optional<const sol::table&> get_table(int npc_id) const;
        std::optional<sol::environment&> get_env(int npc_id);

        void delete_npc_data(int npc_id);
        void delete_filename(const std::string& filename);

    private:
        struct Data {
            std::string filename;
            sol::environment env;
            sol::table data;
        };

        std::unordered_map<int, Data> data;
        std::unordered_multimap<std::string, int> filename_to_id;
};