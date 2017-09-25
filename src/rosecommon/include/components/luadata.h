#pragma once

#include <sol.hpp>

#include <memory>

struct LuaData {
    std::unique_ptr<sol::environment> env_;
};
