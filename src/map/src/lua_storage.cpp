#include "lua_storage.h"

void LuaStorage::register_lua(const std::string& filename, sol::environment env, RoseCommon::Entity) {
    filename_to_entity.emplace(filename, entity);
    Data d;
    d.filename = filename;
    d.env = env;
    data.emplace(entity, d);
    return d.api;
}

void LuaStorage::delete_entity(RoseCommon::Entity entity) {
    if (const auto it = data.find(entity); it != data.end()) {
        const auto range = filename_to_entity.equal_range(it->second.filename);
        for (const auto jt = range.first; jt != range.second; ++jt) {
            if (jt->second == entity) {
                filename_to_id.erase(jt);
                break;
            }
        }
        data.erase(it);
    }
}

std::vector<RoseCommon::Entity> LuaStorage::delete_filename(const std::string& filename) {
    const auto range = filename_to_entity.equal_range(filename);
    std::vector<RoseCommon::Entity> res;
    res.reserve(std::distance(range.first, range.second));
    for (const auto it = range.first; it != range.second; ++it) {
        data.erase(it->second);
        res.push_back(it->second);
    }
    filename_to_entity.erase(range.first, range.second);
    return res;
}