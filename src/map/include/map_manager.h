#pragma once

#include <vector>
#include <thread>

#include "cmapserver.h"
#include "cmapisc.h"

class MapManager {
    public:
        MapManager(std::vector<uint16_t> maps);
    
        void stop();

        ~MapManager();

    private:
        CMapServer isc_server_;
        CMapISC isc_client_;
        std::vector<std::pair<std::thread, CMapServer*>> maps_;
};
