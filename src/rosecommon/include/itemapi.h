#pragma once

#include "luaapi.h"
#include "item.h"

class ItemApi : public LuaApi {
    public:
        ItemApi(RoseCommon::Item& e) : item_(e) {}
        virtual void setupApi(sol::environment& env) {
            (void)env;
        }

    private:
        RoseCommon::Item& item_;
};
