#pragma once

#include <chrono>

namespace { // try to avoid polluting the global namespace
using namespace std::chrono_literals;
}

struct Spawner {
    Spawner(int mob_id, int mob_count, int spawner_limit, std::chrono::seconds spawner_interval, int spawner_range) : 
        mob_id_(mob_id),
        total_on_map_(mob_count),
        total_once_(spawner_limit),
        interval_(spawner_interval),
        range_(spawner_range)
    {}

    int mob_id_;
    int total_on_map_;
    int total_once_;
    int current_total_ = 0;
    std::chrono::seconds interval_;
    std::chrono::seconds until_update_ = 0s;
    int range_;
};
