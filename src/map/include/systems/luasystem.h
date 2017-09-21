#pragma once

#include "system.h"

namespace RoseCommon {
class CliNormalChat;
class CliWhisperChat;
class CliPartyChat;
}

namespace Systems {

class LuaSystem : public System {
    public:
        LuaSystem(SystemManager &manager) : System(manager) {}
        virtual ~LuaSystem() = default;

        virtual void update(EntityManager &es, double dt) {}
};

}
