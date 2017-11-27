#pragma once

#include "sol/environment.hpp"

namespace RoseCommon {

class ItemAPI {
  public:
    ItemAPI(sol::environment&& env) : env_(std::move(env)) {
      // build the C++/lua connectors here
    }
  
    sol::environment& getEnv() { return env_; }
    const sol::environment& getEnv() const { return env_; }
  private:
    sol::environment env_;
};

}
