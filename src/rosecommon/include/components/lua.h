#pragma once

#include <memory>

#include <sol.hpp>

struct Lua {
    Lua() = default;
    Lua(const Lua&) = delete;
    Lua(Lua&& other) : env_(std::move(other.env_)) {}
    Lua& operator=(Lua&& other) { Lua tmp(std::move(other)); std::swap(*this, tmp); return *this; }

    std::unique_ptr<sol::environment> env_;
};
