#pragma once

struct Npc {
    Npc(uint16_t id, uint16_t quest_id) : npc_id_(id), quest_id_(quest_id) {}

    uint16_t npc_id_;
    uint16_t quest_id_;
};
