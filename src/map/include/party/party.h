#pragma once

#include "dataconsts.h"
#include "cli_party_req.h"
#include "cli_party_reply.h"

class EntitySystem;

namespace Party {
void add_member(EntitySystem&, RoseCommon::Entity leader, RoseCommon::Entity member);
void remove_member(EntitySystem&, RoseCommon::Entity);
void change_leader(EntitySystem&, RoseCommon::Entity current_leader, RoseCommon::Entity new_leader);
void process_party_request(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliPartyReq& packet);
void process_party_reply(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliPartyReply& packet);
}
