#pragma once

#include "cli_party_req.h"

class CCharServer;

namespace Party {
void party_request(const RoseCommon::Packet::CliPartyReq& packet, CCharServer& server, uint32_t charId);
}
