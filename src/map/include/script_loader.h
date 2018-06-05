#pragma once

#include "entitysystem.h"

#include <filesystem>

namespace LuaScript {

// finds all lua scripts in the given path (recursive) and add or replace if exists the npcs/monsters/warpgates in the given entity system
void reload_scripts(EntitySystem& entity_system, std::filesystem::path path);

}
