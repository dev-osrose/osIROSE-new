#pragma once

#include "sol/environment.hpp"
#include <memory>
#include "logconsole.h"

namespace RoseCommon {

class EntityAPI {
  public:
    EntityAPI() = default;
    EntityAPI(sol::environment&& env) : env_(std::move(std::make_unique(env))), logger_(Core::CLog::GetLogger(Core::log_type::SYSTEM).lock()) {
      // setup the C++/lua connectors
      luaEnv.env_->set_function("display", [this] (std::string data) {
         logger_->warn("lua display call: {}", data);
      });
    }
  
    sol::environment* getEnv() { return env_.get(); }
    const sol::environment* getEnv() const { return env_.get(); }
  
    void onInit() { auto f = env["onInit"]; f(); }
  
  private:
    std::unique_ptr<sol::environment> env_;
    std::shared_ptr<spdlog::logger> logger_;
};

}
