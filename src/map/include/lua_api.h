#pragma once

#include <sol.hpp>
#include <string>
#include <optional>
#include "logconsole.h"
#include "entity.h"

class LuaApi {
    public:
        LuaApi(sol::environment env) : env(env), logger(Core::CLog::GetLogger(Core::log_type::SYSTEM)) {}

        bool on_init() { return safe_lua_call<bool>("OnInit"); }
        bool on_create() { return safe_lua_call<bool>("OnCreate"); }
        bool on_delete() { return safe_lua_call<bool>("OnDelete"); }

        template <typename T, typename... Args>
        void register_function(const std::string& name, std::function<T(Args...)>&& func) {
            env.set_function(name, std::forward<std::function<T(Args...)>>(func));
        }

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

        bool on_equip(Entity entity) { return safe_lua_call<bool>("OnEquip", entity); }
        bool on_unequip(Entity entity) { return safe_lua_call<bool>("OnUnequip", entity); }
        bool on_drop(Entity entity) { return safe_lua_call<bool>("OnDrop", entity); }
        bool on_pickup(Entity entity) { return safe_lua_call<bool>("OnPickup", entity); }
        bool on_use(Entity entity) { return safe_lua_call<bool>("OnUse", entity); }
        //int  get_use_restriction() { return safe_lua_call<int>("GetUseRestriction"); }
        int  get_attack_speed() { return safe_lua_call<int>("GetAttackSpd"); }
        int  get_move_speed() { return safe_lua_call<int>("GetMoveSpd"); }
        int  get_magic() { return safe_lua_call<int>("GetMagic"); }

        void register_add_bonus_attr(std::function<void(Entity, int, int)>&& func) {
            register_function("addBonusAttr", std::forward<decltype(func)>(func));
        }

        void register_remove_bonus_attr(std::function<void(Entity, int, int)>&& func) {
            register_function("removeBonusAttr", std::forward<decltype(func)>(func));
        }
};

class NpcLuaApi : public LuaApi {
    public:
        NpcLuaApi(sol::environment env) : LuaApi(env) {}

        void on_dead(Entity entity, [[maybe_unused]] Entity attacker) { safe_lua_call<void>("OnDead", entity); }
        void on_damaged(Entity entity, [[maybe_unused]] Entity attacker) { safe_lua_call<void>("OnDamaged", entity); }
};
