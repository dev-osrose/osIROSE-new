#include "itemapi.h"
#include "entitycomponents.h"

using namespace RoseCommon;

ItemAPI::ItemAPI(sol::environment&& env) : LuaAPI(std::move(env)) {
    // build the C++/lua connectors here
    env_.set_function("getAttr", [](void* entity, std::string attr) {
        throw_assert(entity, "The entity cannot be nullptr");    
        //Entity e = *(Entity*)entity;
        (void)attr;
        return 42;
    });
      
    env_.set_function("addBonusAttr", [](void* entity, int32_t attr, int32_t value) {
        throw_assert(entity, "The entity cannot be nullptr");
        //Entity e = *(Entity*)entity;
        (void)attr;
        (void)value;
    });
      
    env_.set_function("removeBonusAttr", [](void* entity, int32_t attr, int32_t value) {
        throw_assert(entity, "The entity cannot be nullptr");
        //Entity e = *(Entity*)entity;
        (void)attr;
        (void)value;
    });
}
