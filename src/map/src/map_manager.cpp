#include "cnetwork_asio.h"
#include "map_manager.h"
#include "platform_defines.h"
#include "config.h"
#include "epackettype.h"

using namespace std::chrono_literals;

MapManager::MapManager(std::vector<uint16_t> maps):
    isc_server_{true}, 
    isc_client_{&isc_server_, std::make_unique<Core::CNetwork_Asio>()},
    maps_{maps.size()} {
    Core::Config& config = Core::Config::getInstance();
    isc_server_.init(config.serverData().iscListenIp, config.mapServer().iscPort);
    isc_server_.listen();

    isc_client_.init(config.mapServer().charIp, config.charServer().iscPort);
    isc_client_.set_type(RoseCommon::to_underlying(RoseCommon::Isc::ServerType::CHAR));
    isc_client_.connect();
    isc_client_.start_recv();

    for (uint16_t id : maps) {
        auto map = std::make_unique<CMapServer>(false, id, &isc_server_);
        map->init(config.serverData().ip, config.mapServer().clientPort + id);
        map->listen();
        CMapServer* map_ptr = map.get();
        maps_.emplace_back(std::pair([this, map = std::move(map)] () {
            map->run();
        }, [this, map_ptr]() {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock);
            map_ptr->stop();
        }));
    }
}

MapManager::~MapManager() {
    cv.notify_all();
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& map : maps_) {
        map.second.join();
        map.first.join();
    }
}
