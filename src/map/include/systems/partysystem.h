#pragma once

#include "system.h"

struct PartyBase;

namespace RoseCommon {
class CliPartyReq;
class CliPartyReply;
}  // namespace RoseCommon

namespace Systems {

class PartySystem : public System {
 public:
  PartySystem(SystemManager&);
  virtual ~PartySystem() = default;

  virtual void update(EntityManager&, std::chrono::milliseconds) override;

  void addPartyMember(Entity leader, Entity newMember);
  void changeLeader(Entity leader, Entity newLeader);

  void processPartyReq(CMapClient& client, Entity entity, const RoseCommon::CliPartyReq& packet);
  void processPartyReply(CMapClient& client, Entity entity, const RoseCommon::CliPartyReply& packet);
};

}  // namespace Systems
