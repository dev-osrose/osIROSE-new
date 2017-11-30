#pragma once

#include "system.h"

#include "item.h"

namespace RoseCommon {
class CliEquipItem;
}

namespace Systems {

class InventorySystem : public System {
    public:
        InventorySystem(SystemManager &manager);
        virtual ~InventorySystem() = default;

        virtual void update(EntityManager &es, double dt);

        static uint8_t findNextEmptySlot(Entity entity);
        static bool swapItems(Entity entity, uint8_t &a, uint8_t &b);

        RoseCommon::Item buildItem(uint16_t id);

        void processEquip(CMapClient& client, Entity entity, const RoseCommon::CliEquipItem &packet);
};

}
