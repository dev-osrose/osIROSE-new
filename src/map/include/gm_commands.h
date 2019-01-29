#pragma once

#include "entity_system.h"
#include "dataconsts.h"

#include <string>

void execute_gm(EntitySystem& entitySystem, RoseCommon::Entity entity, const std::string& message);
