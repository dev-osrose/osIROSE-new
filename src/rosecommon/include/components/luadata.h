#pragma once

#include <memory>

namespace sol {
class environment;
}

struct LuaData {
    std::unique_ptr<sol::environment> env_;
};
