#pragma once

struct Spawner {
    Spawner(int mob_id, int mob_count, int spawner_limit, int spawner_range) : 
        mob_id_(mob_id),
        total_on_map_(mob_count),
        total_once_(spawner_limit),
        range_(spawner_range)
    {}

    int mob_id_;
    int total_on_map_;
    int total_once_;
    int current_total_ = 0;
    int range_;
};
