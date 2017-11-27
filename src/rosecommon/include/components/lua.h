#pragma once

template <typename LuaAPI>
struct Lua {
    Lua() = default;
    Lua(const Lua&) = delete;
    Lua(Lua&& other) : api_(std::move(other.api_)) {}
    Lua& operator=(Lua&& other) { Lua tmp(std::move(other)); std::swap(*this, tmp); return *this; }

    LuaAPI api_;
};
