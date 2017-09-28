#pragma once

#include <memory>

#include <sol.hpp>

struct LuaData {
    std::unique_ptr<sol::environment> env_;
};
