#include "utils/calculation.h"
#include "entity_system.h"

#include "components/basic_info.h"
#include "components/computed_values.h"
#include "components/level.h"
#include "components/life.h"
#include "components/magic.h"
#include "components/stamina.h"
#include "components/lua.h"
#include "components/position.h"
#include "components/skills.h"
#include "components/stats.h"

using namespace RoseCommon;

namespace Calculations {
  float get_runspeed(EntitySystem& entitySystem, RoseCommon::Entity entity) {
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto& values = entitySystem.get_component<Component::ComputedValues>(entity);
    return 0;
  }
  
  int get_maxhp(EntitySystem& entitySystem, RoseCommon::Entity entity) {
    auto& life = entitySystem.get_component<Component::Life>(entity);
    return 0;
  }
  
  int get_maxmp(EntitySystem& entitySystem, RoseCommon::Entity entity) {
    auto& magic = entitySystem.get_component<Component::Magic>(entity);
    return 0;
  }
}