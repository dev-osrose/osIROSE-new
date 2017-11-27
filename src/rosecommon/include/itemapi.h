#pragma once

#include "sol/environment.hpp"

namespace RoseCommon {

class ItemAPI {
  public:
    ItemAPI(sol::environment&& env) : env_(std::move(env)) {
      // build the C++/lua connectors here
    }
  private:
    sol::environment env_;
};

}
