#pragma once

#include <deque>
#include <memory>
#include <atomic>
#include <concepts>
#include "mwsrqueue.h"
#include "crosepacket.h"
#include "entity.h"
#include "entity_system.h"
#include "system_handler.h"

class Map {
    public:
        Map(uint16_t map_id, CMapServer* server);
        EntitySystem(const EntitySystem&) = delete;
        EntitySystem(EntitySystem&&) = default;
        ~EntitySystem() = default;

        // doesn't return until the map is stopped
        void run();
        void stop();

        void send_map(const RoseCommon::CRosePacket& packet) const;
        void send_nearby(Entity entity, const RoseCommon::CRosePacket& packet) const;
        void send_nearby_except_me(Entity entity, const RoseCommon::CRosePacket& packet) const;
        void send_to(Entity entity, const RoseCommon::CRosePacket& packet, bool force = false) const;

        void send_to_entity(Entity entity, Entity other) const;

        void send_to_maps(const RoseCommon::CRosePacket& packet, const std::vector<uint16_t>& maps) const;
        void send_to_char_server(const RoseCommon::CRosePacket& packet, uint32_t charId) const;
        void send_to_chars(const RoseCommon::CRosePacket& packet, const std::vector<std::string>& chars) const;

    private:
        EntitySystem ecs;
        Core::MWSRQueue<std::deque<std::pair<Entity, std::unique_ptr<RoseCommon::CRosePacket>>>> packet_queue;
        RoseCommon::PacketDispatcher<Map&, EntitySystem&, Entity> dispatcher;
        SystemHandler system_handler;
        Nearby nearby;
        LuaLoader lua_loader;
        IdManager idManager;

        std::atomic<bool> is_running;
        std::chrono::steady_clock::duration prev_loop_time;
        std::shared_ptr<spdlog::logger> logger;

        uint16_t map_id;
        bool loading;

        // methods
        void register_dispatchers();

        template <typename F, typename P>
            requires std::invocable<F, Map&, EntitySystem&, Entity, const P&>
        void register_dispatcher(F func);
};

// ----------------------------- template implementations ----------------------------------
template <typename F, typename P>
void EntitySystem::register_dispatcher(F func) {
    dispatcher.add_dispatcher(P::PACKET_ID, std::function{
        [func](const T& packet, Map& map, EntitySystem& es, Entity en) {
            func(map, es, en, packet);
    }});
}
