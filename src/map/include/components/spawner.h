#pragma once

#include "timed_callbacks.h"
#include <vector>
#include "dataconsts.h"
#include <chrono>

namespace Component {
struct Spawner {
    int mob_id;
    int max_mobs;
    int max_once;
    std::chrono::seconds interval;
    int range;

    std::vector<Entity> mobs;

    TimedCallbacks::Wrapper callback;
};
}
