#pragma once

#include "sol/environment.hpp"
#include "logconsole.h"
#include <memory>
#include "throwassert.h"

class EntitySystem;

namespace RoseCommon {

class LuaAPI {
    public:
        LuaAPI() = default;
        LuaAPI(EntitySystem& entitySystem, sol::environment&& env) : entitySystem_(&entitySystem), isCreated_(true), env_(std::move(env)), logger_(Core::CLog::GetLogger(Core::log_type::SYSTEM)) {}

        sol::environment& getEnv() { 
            throw_assert(isCreated_, "Error, trying to access the sol environment before creating it");
            return env_;
        }

        const sol::environment getEnv() const {
            throw_assert(isCreated_, "Error, trying to access the sol environment before creating it");
            return env_;
        }

    protected:
        EntitySystem* entitySystem_ = nullptr;
        bool isCreated_ = false;
        sol::environment env_;
        std::shared_ptr<spdlog::logger> logger_;
};

}
