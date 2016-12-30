#pragma once

#include "system.h"

class PartyBase;

namespace Systems {

class PartySystem : public System {
    public:
        PartySystem(SystemManager&);
        virtual ~PartySystem() = default;

        virtual void update(EntityManager&, double);

        void processPartyReq(CMapClient *client, Entity entity, const RoseCommon::CliPartyReq &packet);
        void processPartyReply(CMapClient *client, Entity entity, const RoseCommon::CliPartyReply &packet);
};

}
