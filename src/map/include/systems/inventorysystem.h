#pragma once

#include "system.h"

#include "item.h"
#include <optional>

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

        static bool addItem(Entity entity, RoseCommon::Item&& item);
        static RoseCommon::Item removeItem(Entity entity, uint8_t slot);

        std::optional<RoseCommon::Item> buildItem(uint8_t type, uint16_t id, uint16_t life = 1000, bool isAppraised = true);

        void processEquip(CMapClient& client, Entity entity, const RoseCommon::CliEquipItem &packet);
};

}
