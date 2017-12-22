#include "itemapi.h"
#include "entitycomponents.h"

using namespace RoseCommon;

ItemAPI::ItemAPI(sol::environment&& env) : LuaAPI(std::move(env)) {
      // build the C++/lua connectors here
      env_.set_function("getAttr", [this](void* entity, std::string attr) {
        throw_assert(entity, "The entity cannot be nullptr");    
        Entity e = *(Entity*)entity;
        logger_->info("getAttr called for client {} and attr {}", getId(e), attr);
      });
    }
