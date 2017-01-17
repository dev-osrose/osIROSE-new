#pragma once

#include "system.h"

struct PartyBase;

namespace Systems {

class PartySystem : public System {
    public:
        PartySystem(SystemManager&);
        virtual ~PartySystem() = default;

        virtual void update(EntityManager&, double);

        void addPartyMember(Entity leader, Entity newMember);
        void changeLeader(Entity leader, Entity newLeader);

        void processPartyReq(std::shared_ptr<CMapClient> client, Entity entity, const RoseCommon::CliPartyReq &packet);
        void processPartyReply(std::shared_ptr<CMapClient> client, Entity entity, const RoseCommon::CliPartyReply &packet);
};

}
