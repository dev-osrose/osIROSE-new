#include "systemmanager.h"
#include "systems/system.h"

void SystemManager::update(double dt) {
    for (auto &it : systems_)
        it.second->update(entityManager_, dt);
}
