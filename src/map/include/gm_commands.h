#pragma once

#include "entity_system.h"
#include "dataconsts.h"

#include <string>

void execute_gm(EntitySystem& entitySystem, Entity entity, const std::string& message);
