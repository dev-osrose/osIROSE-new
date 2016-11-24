#pragma once

#include "system.h"

namespace Systems {

class ChatSystem : public System {
    public:
        ChatSystem(SystemManager &manager);
        virtual ~ChatSystem() = default;

        virtual void update(EntityManager &es, double dt);

        void normalChat(CMapClient *client, Entity entity, const RoseCommon::CliNormalChat &packet);
        void whisperChat(CMapClient *client, Entity entity, const RoseCommon::CliWhisperChat &packet);
};

}
