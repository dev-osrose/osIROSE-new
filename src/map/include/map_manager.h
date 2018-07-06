#pragma once

#include <vector>
#include <thread>
#include <atomic>

#include "cmapserver.h"

class MapManager {
    public:
        MapManager(std::vector<i16> maps);

        ~MapManager();

    private:
        CMapServer isc_server_;
        std::vector<std::thread> maps_;
        std::atomic<bool> stop_;
};
