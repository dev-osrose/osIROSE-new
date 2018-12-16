#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "cmapserver.h"
#include "cmapisc.h"

class MapManager {
    public:
        MapManager(std::vector<uint16_t> maps);

        ~MapManager();

    private:
        CMapServer isc_server_;
        CMapISC isc_client_;
        std::vector<std::pair<std::thread, std::thread>> maps_;
        std::mutex mutex;
        std::condition_variable cv;
};
