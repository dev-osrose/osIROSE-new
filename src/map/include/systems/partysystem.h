#pragma once

#include "system.h"

class PartyBase;

namespace Systems {

class PartySystem : public System {
    public:
        PartySystem(SystemManager&);
        virtual ~PartySystem() = default;

        virtual void update(EntityManager&, double);

        enum addToPartyResult {
            ERROR,
            FULL,
            INVALID_LEVEL,
            CREATED,
            ADDED
        };
        
        // this updates the internal model and sends the corresponding packets
        void addPartyMember(Entity currentMember, Entity newMember);

        // this updates the internal model and sends the corresponding packets
        void removePartyMember(std::shared_ptr<PartyBase> party, Entity memberToRemove);

        // this updates the internal model and sends the corresponding packets
        void changePartyLeader(std::shared_ptr<PartyBase> party, Entity newLeader);

        // this updates the internal model and sends the corresponding packets
        void kickPartyMember(std::shared_ptr<PartyBase> party, Entity member);

        void processPartyReq(CMapClient *client, Entity entity, const RoseCommon::CliPartyReq &packet);
        void processPartyReply(CMapClient *client, Entity entity, const RoseCommon::CliPartyReply &packet);

    private:
        // this just updates the internal model
        static addToPartyResult addToParty(Entity member, Entity newMember);
};

}
