#pragma once

#include "sol/environment.hpp"
#include <memory>

namespace RoseCommon {

class ItemAPI {
  public:
    ItemAPI() = default;
    ItemAPI(sol::environment&& env) : env_(std::make_unique(std::move(env))) {
      // build the C++/lua connectors here
    }
  
    sol::environment* getEnv() { return env_.get(); }
    const sol::environment* getEnv() const { return env_.get(); }
  private:
    std::unique_ptr<sol::environment> env_;
};

}
