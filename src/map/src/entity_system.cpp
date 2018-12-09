#include "entity_system.h"

EntitySystem::EntitySystem(std::chrono::milliseconds maxTimePerUpdate) : maxTimePerUpdate(maxTimePerUpdate) {
	logger = Core::CLog::GetLogger(Core::log_type::NETWORK).lock();
}

EntitySystem::~EntitySystem() {
    work_queue.kill();
}

void EntitySystem::add_task(std::function<void(RoseCommon::Registry&, std::chrono::milliseconds)>&& task) {
    work_queue.push_back(std::move(task));
}

void EntitySystem::update(std::chrono::milliseconds dt) {
    auto start = Core::Time::GetTickCount();
    for (auto [res, task] = work_queue.pop_front(); res;) {
        task(registry, dt);
        const std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(Core::Time::GetTickCount() - start);
        if (diff >= maxTimePerUpdate) {
            logger->warn("Stopping after {}ms, {} tasks remaining", maxTimePerUpdate.count(), work_queue.size());
            break;
        }
        auto [tmp_res, tmp_task] = work_queue.pop_front();
        res = tmp_res;
        task = std::move(tmp_task);
    }
}
