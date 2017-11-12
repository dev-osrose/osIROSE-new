#pragma once

#include "luaapi.h"

class EntityApi : public LuaApi {
    public:
        EntityApi(Entity e) : entity_(e) {}
        virtual void setupApi(sol::environment& env) {
            env.set_function("testCpp", [this] () {
                        auto basic = entity_.component<BasicInfo>();
                        return basic->name_;
                    });
        }

    private:
        Entity entity_;
};
