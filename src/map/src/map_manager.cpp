#include "cnetwork_asio.h"
#include "map_manager.h"
#include "platform_defines.h"
#include "config.h"
#include "epackettype.h"
#include "rose_ssl_config.h"

using namespace std::chrono_literals;

MapManager::MapManager(std::vector<uint16_t> maps):
    isc_server_{true},
    isc_client_{&isc_server_, std::make_unique<Core::CNetwork_Asio>()} {
    Core::Config& config = Core::Config::getInstance();
    // reserve(), not vector(count): the latter default-constructs that many
    // entries with a null CMapServer*, which the emplace_back below then
    // appends to - leaving half the vector null.
    maps_.reserve(maps.size());
    //isc_server_.init(config.serverData().iscListenIp, config.mapServer().iscPort);
    //isc_server_.listen();

    isc_client_.add_maps(maps);
    isc_client_.init(config.mapServer().charIp, config.charServer().iscPort);
    isc_client_.set_type(RoseCommon::to_underlying(RoseCommon::Isc::ServerType::CHAR));
    if (!RoseCommon::ApplySslClientConfig(isc_client_, config, config.mapServer().charIp,
                                          "map -> char ISC"))
        throw std::runtime_error("Could not configure TLS for the map -> char ISC connection");
    // CRoseISC::onConnected() arms the read once the handshake has resolved,
    // so there is no start_recv() call here.
    if (!isc_client_.connect_and_wait())
        throw std::runtime_error("Could not establish the ISC connection to the char server");

    for (uint16_t id : maps) {
        auto map = std::make_unique<CMapServer>(false, id, &isc_server_, &isc_client_);
        map->init(config.serverData().listenIp, config.mapServer().clientPort + id);
        if (!RoseCommon::ApplySslServerConfig(*map, config, "map client"))
            throw std::runtime_error("Could not configure TLS for a map listener");
        if (!map->listen())
            throw std::runtime_error("Could not listen on a map port");
        isc_client_.register_map(id, map->get_entity_system());
        CMapServer* map_ptr = map.get();
        maps_.emplace_back(std::pair([this, map = std::move(map)] () {
            map->run();
        }, map_ptr));
    }
}

MapManager::~MapManager() {
    if (maps_.size()) {
        stop();
    }
}

void MapManager::stop() {
    for (auto& map : maps_) {
        if (map.second) {
            map.second->stop();
            map.first.join();
        }
    }
    maps_.clear();
}
