#pragma once

#include <memory>

class sol::environment;

struct LuaData {
    std::unique_ptr<sol::environment> env_;
};
