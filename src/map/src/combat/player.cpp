//ALL ADDED BY DAVIDIXX

#include "dataconsts.h"
#include "combat/player.h"
#include "combat/combat.h"
#include "entity_system.h"
#include "components/basic_info.h"
#include "components/stats.h"
#include "cli_stat_add_req.h"
#include "srv_stat_add_reply.h"
#include <cmath>

using namespace RoseCommon;
using namespace Player;

void Player::add_stat(EntitySystem& entitySystem, RoseCommon::Entity entity, const RoseCommon::Packet::CliStatAddReq& packet) {
	auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
	auto& stats = entitySystem.get_component<Component::Stats>(entity);
	uint8_t statId = packet.get_stat();
	uint32_t neededStatPoints = 0;
	uint16_t newStatValue = 0;
	// Calculating the needed points for a stat raise
	switch(statId)
    {
		case 0: neededStatPoints = (int)floor((float)stats.str / 5); break;
		case 1: neededStatPoints = (int)floor((float)stats.dex / 5); break;
		case 2: neededStatPoints = (int)floor((float)stats.int_ / 5); break;
		case 3: neededStatPoints = (int)floor((float)stats.con / 5); break;
		case 4: neededStatPoints = (int)floor((float)stats.charm / 5); break;
		case 5: neededStatPoints = (int)floor((float)stats.sense / 5); break;
	}

	// Is there enough stat points AND stat is not maxed?
	if (basicInfo.statPoints >= neededStatPoints && neededStatPoints <(MAX_STAT / 5)) {
		switch(statId)
        {
			case 0: newStatValue = ++stats.str; break;
			case 1: newStatValue = ++stats.dex; break;
			case 2: newStatValue = ++stats.int_; break;
			case 3: newStatValue = ++stats.con; break;
			case 4: newStatValue = ++stats.charm; break;
			case 5: newStatValue = ++stats.sense; break;
		}

		// Updating HP & MP
		Combat::updateStats(entitySystem, entity);
		auto pStat = RoseCommon::Packet::SrvStatAddReply::create(static_cast<Packet::SrvStatAddReply::Stat>(statId), newStatValue);
		entitySystem.send_to(entity, pStat);
	}

}
