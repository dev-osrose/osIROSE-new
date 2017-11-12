#pragma once

#include <sol.hpp>

class LuaApi {
    public:
        virtual void setupApi(sol::environment& env) = 0;
        virtual ~LuaApi() = default;
};
