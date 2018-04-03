#pragma once

#include "sol/environment.hpp"
#include "logconsole.h"
#include <memory>
#include "throwassert.h"
#include <optional>

namespace RoseCommon {

class LuaAPI {
    public:
        LuaAPI() : isCreated_(false) {}
        virtual ~LuaAPI() = default;
        LuaAPI(sol::environment&& env) : isCreated_(true), env_(std::move(env)), logger_(Core::CLog::GetLogger(Core::log_type::SYSTEM)) {}

        sol::environment& getEnv() { 
            throw_assert(isCreated_, "Error, trying to access the sol environment before creating it");
            return env_;
        }

        const sol::environment getEnv() const {
            throw_assert(isCreated_, "Error, trying to access the sol environment before creating it");
            return env_;
        }

    protected:
        bool isCreated_;
        sol::environment env_;
        std::shared_ptr<spdlog::logger> logger_;

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
