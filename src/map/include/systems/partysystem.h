#pragma once

#include "system.h"

namespace Systems {

class PartySystem : public System {
    public:
        PartySystem(SystemManager&);
        virtual ~PartySystem() = default;

        virtual void update(EntityManager&, double);

        // first = isOk, second = isNew
        static std::pair<bool, bool> addToParty(Entity member, Entity newMember);

        void processPartyReq(CMapClient *client, Entity entity, const RoseCommon::CliPartyReq &packet);
        void processPartyReply(CMapClient *client, Entity entity, const RoseCommon::CliPartyReply &packet);
};

}
