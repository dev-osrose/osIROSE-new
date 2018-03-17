#pragma once

#include "luaapi.h"

namespace RoseCommon {

class ItemAPI : public LuaAPI {
  public:
    ItemAPI() = default;
    virtual ~ItemAPI() { if (isCreated_) onDelete(); }
    ItemAPI(sol::environment&& env);

    void onInit() { safeVoidLuaCall("OnInit"); }
    void onDelete() { safeVoidLuaCall("OnDelete"); }
    bool onEquip(void* entity) { return safeLuaCall<bool>("OnEquip", entity); }
    bool onUnequip(void* entity) { return safeLuaCall<bool>("OnUnequip", entity); }
    bool onDrop(void* entity) { return safeLuaCall<bool>("OnDrop", entity); }
    bool onPickup(void* entity) { return safeLuaCall<bool>("OnPickup", entity); }
    bool onUse(void* entity) { return safeLuaCall<bool>("OnUse", entity); }

    template <typename T, typename... Args>
    T safeLuaCall(const std::string& func, const Args&... args) {
        logger_->trace("Calling lua function {}", func);
        if (!isCreated_) {
            logger_->warn("Trying to call lua function {}() when the lua environment hasn't been created", func);
            return {};
        }
        sol::protected_function f = env_[func];
        auto res = f(args...);
        if (res.valid())
            return static_cast<T>(res);
        sol::error err = res;
        logger_->error("{}() lua call failed with: {}", func, err.what());
        return {};
    }

    template <typename... Args>
    void safeVoidLuaCall(const std::string& func, const Args... args) {
        logger_->trace("Calling lua function {}", func);
        if (!isCreated_) {
            logger_->warn("Trying to call lua function {}() when the lua environment hasn't been created", func);
            return;
        }
        sol::protected_function f = env_[func];
        auto res = f(args...);
        if (res.valid())
            return;
        sol::error err = res;
        logger_->error("{}() lua call failed with: {}", func, err.what());
        return;
    }
};

}
