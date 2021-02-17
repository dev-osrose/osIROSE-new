#include "dataconsts.h"
#include "cmapclient.h"
#include "combat/player.h"
#include "combat/combat.h"
#include "entity_system.h"
#include "components/basic_info.h"
#include "components/stats.h"
#include "components/computed_values.h"
#include "cli_stat_add_req.h"
#include "cli_toggle_move.h"
#include "cli_set_animation.h"
#include "srv_stat_add_reply.h"
#include "srv_toggle_move.h"
#include "srv_set_animation.h"
#include <cmath>
#include "utils/calculation.h"


using namespace RoseCommon;
using namespace Player;

void Player::add_stat(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliStatAddReq& packet) {
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
	if (basicInfo.statPoints >= neededStatPoints && neededStatPoints < (MAX_STAT / 5)) {
		switch(statId)
        {
			case 0: newStatValue = ++stats.str; break;
			case 1: newStatValue = ++stats.dex; break;
			case 2: newStatValue = ++stats.int_; break;
			case 3: newStatValue = ++stats.con; break;
			case 4: newStatValue = ++stats.charm; break;
			case 5: newStatValue = ++stats.sense; break;
		}
		basicInfo.statPoints -= neededStatPoints;
		// Updating HP & MP
		Combat::updateStats(entitySystem, entity);
		auto pStat = Packet::SrvStatAddReply::create(static_cast<Packet::SrvStatAddReply::Stat>(statId), newStatValue);
		entitySystem.send_to(entity, pStat);
	}

}

void Player::toggle_player_move(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliToggleMove& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();

	Packet::CliToggleMove::ToggleMove moveType = packet.get_type();
	auto& computedValues = entitySystem.get_component<Component::ComputedValues>(entity);
	if(moveType == 0) {
		if (computedValues.moveMode == MoveMode::RUN) {
			computedValues.moveMode = MoveMode::WALK;
		} else if (computedValues.moveMode == MoveMode::WALK) {
			computedValues.runSpeed = Calculations::get_runspeed(entitySystem, entity); //get real run speed
			computedValues.moveMode = MoveMode::RUN;
		}
	}
	
	auto pToggle = Packet::SrvToggleMove::create(static_cast<Packet::SrvToggleMove::ToggleMove>(computedValues.moveMode));
	pToggle.set_run_speed(computedValues.runSpeed);
	auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
	pToggle.set_object_id(basicInfo.id);
	entitySystem.send_map(pToggle);
}

void Player::set_animation(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliSetAnimation& packet) {
	const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
	auto pAnimation = Packet::SrvSetAnimation::create();
	pAnimation.set_id(packet.get_id());
	pAnimation.set_value(packet.get_value());
	pAnimation.set_object_id(basicInfo.id);
	entitySystem.send_nearby(entity, pAnimation);
}