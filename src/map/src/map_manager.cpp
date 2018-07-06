#include "map_manager.h"
#include "platform_defines.h"
#include "cmapisc.h"
#include "config.h"

MapManager::MapManager(std::vector<i16> maps):
    isc_server_{true}, maps_{maps.size()}, stop_(false) {
    Core::Config& config = Core::Config::getInstance();
    isc_server_.init(config.serverData().iscListenIp, config.mapServer().iscPort);
    isc_server_.listen();
    for (i16 id : maps) {
        auto server = std::make_unique<CMapServer>(false, id);
        CMapISC* iscClient = new CMapISC(server.get(), std::make_unique<Core::CNetwork_Asio>());
        iscClient->init(config.mapServer().charIp, config.charServer().iscPort);
        iscClient->set_type(to_underlying(RoseCommon::Isc::ServerType::CHAR));
        server->init(config.serverData().ip, config.mapServer().clientPort + id);
        server->listen();
        server->GetISCList().push_front(std::unique_ptr<CMapISC>(iscClient));
        iscClient->connect();
        iscClient->start_recv();
        maps_.emplace_back([this, std::move(server)] () {
            auto start = Core::Time::GetTickCount();
            while (!stop_.load() && server.is_active()) {
                std::chrono::duration<double> diff = Core::Time::GetTickCount() - start;
                server.update(diff.count());
                start = Core::Time::GetTickCount();
                std::this_thread__sleep_for(std::chrono::milliseconds(100));
            }
        });
    }
}

void MapManager::~MapManager() {
    stop_.store(true);
    for (auto& map : maps_) {
        map.join();
    }
}
