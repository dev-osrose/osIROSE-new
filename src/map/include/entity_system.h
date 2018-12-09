#pragma once

#include <functional>
#include <chrono>
#include <queue>

#include "dataconsts.h"
#include "logconsole.h"
#include "mwsrqueue.h"

using namespace std::chrono_literals;

class EntitySystem {
    public:
        EntitySystem(std::chrono::milliseconds maxTimePerUpdate = 50ms);
        ~EntitySystem();

        void update(std::chrono::milliseconds);

        void add_task(std::function<void(RoseCommon::Registry&, std::chrono::milliseconds)>&& task);

    private:
        Core::MWSRQueue<std::deque<std::function<void(RoseCommon::Registry&, std::chrono::milliseconds)>>> work_queue;

        RoseCommon::Registry registry;

        std::shared_ptr<spdlog::logger> logger;

		std::chrono::milliseconds maxTimePerUpdate;
};
