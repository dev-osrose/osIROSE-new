#pragma once

#include <vector>
#include <chrono>
#include <functional>

namespace {
using namespace std::chrono_literals;
}

class SystemManager;

struct CallbackImp {
    CallbackImp(std::function<bool(SystemManager&)>&& callback, std::chrono::milliseconds update):
        callback_(std::move(callback)),
        time_to_update_(update) {}

    std::function<bool(SystemManager&)> callback_;
    std::chrono::milliseconds time_to_update_;
    std::chrono::milliseconds time_elapsed_ = 0ms;
};

struct Callback {
    std::vector<CallbackImp> callbacks_;
};
