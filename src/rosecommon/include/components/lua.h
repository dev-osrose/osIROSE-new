#pragma once

#include <memory>

#include <sol.hpp>

struct Lua {
    std::unique_ptr<sol::environment> env_;
};
