#pragma once

#include <memory>

#include "luaapi.h"

struct Lua {
    Lua() = default;
    Lua(const Lua&) = delete;
    Lua(Lua&& other) : env_(std::move(other.env_)), api_(std::move(other.api_)) {}
    Lua& operator=(Lua&& other) { Lua tmp(std::move(other)); std::swap(*this, tmp); return *this; }

    std::unique_ptr<sol::environment> env_;
    std::unique_ptr<LuaApi> api_;
};
