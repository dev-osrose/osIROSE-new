#pragma once

#include <sol.hpp>
#include <string>
#include "logconsole.h"
#include "dataconsts.h"

class LuaApi {
    public:
        LuaApi(sol::environment env) : env(env), logger(Core::CLog::GetLogger(Core::log_type::SYSTEM)) {}

        bool on_init() { return safe_lua_call<bool>("OnInit"); }
        bool on_create() { return safe_lua_call<bool>("OnCreate"); }
        bool on_delete() { return safe_lua_call<bool>("OnDelete"); }
    
    protected:
        template <typename T, typename... Args>
        T safe_lua_call(const std::string& name, Args&&... args) {
            logger->trace("Calling lua function {}", name);
            sol::protected_function f = env[name];
            auto res = f(std::forward<Args>(args)...);
            if constexpr (std::is_same_v<T, void>) {
                if (!res.valid()) {
                    sol::error err = res;
                    logger->error("Error: {}() lua call failed with: {}", name, err.what());
                }
            } else {
                if (res.valid()) {
                    return static_cast<T>(res);
                }
                sol::error err = res;
                logger->error("Error: {}() lua call failed with: {}", name, err.what());
                return {};
            }
        }

    private:
        sol::environment env;
        std::shared_ptr<spdlog::logger> logger;
};

class ItemLuaApi : public LuaApi {
    public:
        ItemLuaApi(sol::environment env) : LuaApi(env) {}

        bool on_equip(RoseCommon::Entity entity) { return safe_lua_call<bool>("OnEquip", entity); }
        bool on_unequip(RoseCommon::Entity entity) { return safe_lua_call<bool>("OnUnequip", entity); }
        bool on_drop(RoseCommon::Entity entity) { return safe_lua_call<bool>("OnDrop", entity); }
        bool on_pickup(RoseCommon::Entity entity) { return safe_lua_call<bool>("OnPickup", entity); }
        bool on_use(RoseCommon::Entity entity) { return safe_lua_call<bool>("OnUse", entity); }
};

class NpcLuaApi : public LuaApi {
    public:
        NpcLuaApi(sol::environment env) : LuaApi(env) {}

        void on_dead(RoseCommon::Entity entity) { safe_lua_call<void>("OnDead", entity); }
        void on_damaged(RoseCommon::Entity entity) { safe_lua_call<void>("OnDamaged", entity); }
};
