#pragma once

#include "system.h"

namespace Systems {

class ChatSystem : public System {
    public:
        ChatSystem(SystemManager &manager);
        virtual ~ChatSystem() = default;

        virtual void update(EntityManager &es, double dt);

        void sendMsg(Entity entity, const std::string &msg);

        void normalChat(std::shared_ptr<CMapClient> client, Entity entity, const RoseCommon::CliNormalChat &packet);
        void whisperChat(std::shared_ptr<CMapClient> client, Entity entity, const RoseCommon::CliWhisperChat &packet);
        void partyChat(std::shared_ptr<CMapClient> client, Entity entity, const RoseCommon::CliPartyChat &packet);
};

}
