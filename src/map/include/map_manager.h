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

        /// Whether the ISC link up to the character server is currently connected.
        [[nodiscard]] bool is_isc_connected() const {
            return isc_client_.is_connected();
        }

        /// Whether every per-map client listener is still accepting.
        /// One dead map leaves the process running but unable to serve that map,
        /// which is exactly the state the health check needs to surface.
        [[nodiscard]] bool are_maps_listening() const {
            if (maps_.empty()) return false;
            for (const auto& map : maps_) {
                if (!map.second || !map.second->is_active()) return false;
            }
            return true;
        }

    private:
        CMapServer isc_server_;
        CMapISC isc_client_;
        std::vector<std::pair<std::thread, CMapServer*>> maps_;
};
