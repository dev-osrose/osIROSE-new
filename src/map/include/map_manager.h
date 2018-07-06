#pragma once

#include <vector>
#include <thread>
#include <atomic>

#include "cmapserver.h"
#include "cmapisc.h"

class MapManager {
    public:
        MapManager(std::vector<uint16_t> maps);

        ~MapManager();

    private:
        CMapServer isc_server_;
        CMapISC isc_client_;
        std::vector<std::thread> maps_;
        std::atomic<bool> stop_;
};
