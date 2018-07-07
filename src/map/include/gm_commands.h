#pragma once

#include "entitycomponents.h"

#include <string>

class SystemManager;

// return true if a command was executed, false otherwise
bool executeGM(const std::string& command, SystemManager& manager, Entity entity);
