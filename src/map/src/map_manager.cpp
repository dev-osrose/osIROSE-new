#include "cnetwork_asio.h"
#include "map_manager.h"
#include "platform_defines.h"
#include "config.h"

MapManager::MapManager(std::vector<uint16_t> maps):
    isc_server_{true}, 
    isc_client_{&isc_server_, std::make_unique<Core::CNetwork_Asio>()},
    maps_{maps.size()},
    stop_(false) {
    Core::Config& config = Core::Config::getInstance();
    isc_server_.init(config.serverData().iscListenIp, config.mapServer().iscPort);
    isc_server_.listen();

    isc_client_.init(config.mapServer().charIp, config.charServer().iscPort);
    isc_client_.set_type(to_underlying(RoseCommon::Isc::ServerType::CHAR));
    isc_client_.connect();
    isc_client_.start_recv();

    for (uint16_t id : maps) {
        auto map = std::make_unique<CMapServer>(false, id, &isc_server_);
        map->init(config.serverData().ip, config.mapServer().clientPort + id);
        map->listen();
        maps_.emplace_back([this, map = std::move(map)] () {
            auto start = Core::Time::GetTickCount();
            while (!stop_.load() && map->is_active()) {
                std::chrono::duration<double> diff = Core::Time::GetTickCount() - start;
                map->update(diff.count());
                start = Core::Time::GetTickCount();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }
}

MapManager::~MapManager() {
    stop_.store(true);
    for (auto& map : maps_) {
        map.join();
    }
}
