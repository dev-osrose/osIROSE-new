#include "lua_db.h"

void LuaDb::store_lua(const std::string& filename, sol::environment env, int npc_id, sol::table npc_data) {
    filename_to_id.emplace(filename, npc_id);
    Data d;
    d.filename = filename;
    d.env = env;
    d.data = npc_data;
    d.api = std::make_shared<NpcLuaApi>(env);
    data.emplace(npc_id, d);
}

std::weak_ptr<NpcLuaApi> LuaDb::get_lua_api(int npc_id) {
    if (const auto it = data.find(npc_id); it != data.end()) {
        return it->second.api;
    }
    return {};
}

std::optional<LuaDb::LuaData> LuaDb::get_data(int npc_id) const {
    if (const auto it = data.find(npc_id); it != data.end()) {
        return {{it->second.data}};
    }
    return {};
}

void LuaDb::delete_npc_data(int npc_id) {
    if (const auto it = data.find(npc_id); it != data.end()) {
        const auto range = filename_to_id.equal_range(it->second.filename);
        for (const auto jt = range.first; jt != range.second; ++jt) {
            if (jt->second == npc_id) {
                filename_to_id.erase(jt);
                break;
            }
        }
        data.erase(it);
    }
}

void LuaDb::delete_filename(const std::string& filename) {
    const auto range = filename_to_id.equal_range(filename)
    for (const auto it = range.first; it != range.second; ++it) {
        data.erase(it->second);
    }
    filename_to_id.erase(range.first, range.second);
}