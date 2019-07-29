#pragma once

#include <string>
#include <optional>
#include "dataconsts.h"

class EntitySystem;

namespace Calculations {
  float get_runspeed(EntitySystem&, RoseCommon::Entity);
  int get_weight(EntitySystem&, RoseCommon::Entity);
  
  int get_maxhp(EntitySystem&, RoseCommon::Entity);
  int get_maxmp(EntitySystem&, RoseCommon::Entity);
}
