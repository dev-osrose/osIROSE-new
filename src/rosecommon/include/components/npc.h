#pragma once

struct Npc {
    Npc(uint16_t id) : npc_id_(id) {}

    uint16_t npc_id_;
    uint16_t quest_id_ = 0;
};
