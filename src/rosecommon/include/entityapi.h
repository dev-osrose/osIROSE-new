#pragma once

#include "sol/environment.hpp"
#include <memory>

namespace RoseCommon {

class EntityAPI {
  public:
    EntityAPI() = default;
    EntityAPI(sol::environment&& env) : env_(std::move(std::make_unique(env))) {}
  
    sol::environment* getEnv() { return env_.get(); }
    const sol::environment* getEnv() const { return env_.get(); }
  
    void onInit() { auto f = env["onInit"]; f(); }
  
  private:
    std::unique_ptr<sol::environment> env_;
};

}
