#pragma once

#include "system.h"

namespace Systems {

class ChatSystem : public System {
    public:
        ChatSystem(EntityManager &es, SystemManager &manager);
        virtual ~ChatSystem() = default;

        virtual void update(EntityManager &es, double dt);

        void normalChat(EntityManager &es, CMapClient *client, Entity entity, const RoseCommon::CliNormalChat &packet);
        void whisperChat(EntityManager &es, CMapClient *client, Entity entity, const RoseCommon::CliWhisperChat &packet);
};

}
