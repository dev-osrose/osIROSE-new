#pragma once

template <typename LuaAPI>
struct LuaComponent {
    LuaComponent(LuaAPI&& api) : api_(std::move(api)) {}
    LuaComponent(const LuaComponent&) = delete;
    LuaComponent(LuaComponent&& other) : api_(std::move(other.api_)) {}
    LuaComponent& operator=(LuaComponent&& other) { LuaComponent<LuaAPI> tmp(std::move(other)); std::swap(*this, tmp); return *this; }

    LuaAPI api_;
};
