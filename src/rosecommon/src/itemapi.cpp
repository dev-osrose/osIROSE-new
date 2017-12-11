#include "itemapi.h"
#include "entitysystem.h"

using namespace RoseCommon;

ItemAPI::ItemAPI(EntitySystem& es, sol::environment&& env) : LuaAPI(es, std::move(env)) {
      // build the C++/lua connectors here
      env_.set_function("getAttr", [this](uint32_t charId, std::string attr) {
        Entity e = entitySystem->getEntity(charId);
        logger_->info("getAttr called for client {} and attr {}", getId(e), attr);
      });
    }
