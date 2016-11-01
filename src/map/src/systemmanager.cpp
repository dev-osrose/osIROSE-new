#include "systemmanager.h"
#include "entitySystems.h"

void SystemManager::update(double dt) {
    for (auto &it : systems_)
        it->update(entityManager_, dt);
}
