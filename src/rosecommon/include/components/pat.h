#pragma once

#include <chrono>

namespace {
using namespace std::chrono_literals;
}

namespace Component {
struct Pat {
    uint16_t hp;
    uint16_t maxHp
    std::chrono::seconds cooldowTime;
    std::chrono::milliseconds dt = 0ms;
};
}
