#pragma once

#include "system.h"

namespace Systems {

class MapSystem : public System {
    public:
        MapSystem(SystemManager&);
        virtual ~MapSystem() = default;

        virtual void update(EntityManager&, double);

        void processChangeMapRequest(CMapClient *client, Entity entity, const RoseCommon::CliChangeMapReq &packet);
};

}
