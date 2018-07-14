#pragma once

#include "system.h"

namespace RoseCommon {
class CliChangeMapReq;
}

namespace Systems {

class MapSystem : public System {
 public:
  MapSystem(SystemManager&);
  virtual ~MapSystem() = default;

  virtual void update(EntityManager&, std::chrono::milliseconds) override;

  void processChangeMapReq(CMapClient& client, Entity entity, const RoseCommon::CliChangeMapReq& packet);
};

}  // namespace Systems
