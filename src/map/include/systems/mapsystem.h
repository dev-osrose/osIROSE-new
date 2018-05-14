#pragma once

#include <packets/cli_changemapreq.h>
#include "system.h"

namespace RoseCommon {
class CliChangeMapReq;
}

namespace Systems {

class MapSystem : public System {
 public:
  MapSystem(SystemManager&);
  virtual ~MapSystem() = default;

  virtual void update(EntityManager&, double);

  void processChangeMapReq(CMapClient& client, Entity entity, const RoseCommon::CliChangeMapReq& packet);
};

}  // namespace Systems
