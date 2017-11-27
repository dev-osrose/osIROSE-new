#pragma once

#include "sol/environment.hpp"
#include "logconsole.h"
#include <memory>

namespace RoseCommon {

class ItemAPI {
  public:
    ItemAPI() = default;
    ItemAPI(sol::environment&& env) : env_(std::make_unique(std::move(env))), logger_(Core::CLog::GetLogger(Core::log_type::SYSTEM).lock()) {
      // build the C++/lua connectors here
    }
  
    sol::environment* getEnv() { return env_.get(); }
    const sol::environment* getEnv() const { return env_.get(); }
  private:
    std::unique_ptr<sol::environment> env_;
    std::shared_ptr<spdlog::logger> logger_;
};

}
