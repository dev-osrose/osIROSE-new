#pragma once

#include "luaapi.h"
#include <optional>

namespace RoseCommon {

class ItemAPI : public LuaAPI {
  public:
    ItemAPI() = default;
    virtual ~ItemAPI() = default;
    ItemAPI(sol::environment&& env);

    void onInit() { safeLuaCall<bool>("OnInit"); }
    void onDelete() { safeLuaCall<bool>("OnDelete"); }
    bool onEquip(void* entity) { return safeLuaCall<bool>("OnEquip", entity).value_or(false); }
    bool onUnequip(void* entity) { return safeLuaCall<bool>("OnUnequip", entity).value_or(false); }
    bool onDrop(void* entity) { return safeLuaCall<bool>("OnDrop", entity).value_or(false); }
    bool onPickup(void* entity) { return safeLuaCall<bool>("OnPickup", entity).value_or(false); }
    bool onUse(void* entity) { return safeLuaCall<bool>("OnUse", entity).value_or(false); }

    template <typename T, typename... Args>
    std::optional<T> safeLuaCall(const std::string& func, const Args&... args) {
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
};

}
