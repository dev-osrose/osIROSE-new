#include "entitySystem.h"
#include "database.h"

Entity EntitySystem::loadCharacter(uint32_t charId, bool platinium) {
    auto &database = Core::databasePool.getDatabase();
    auto res = database.QStore(fmt::format("CALL GetCharacter({});", charId));
    auto entity = create();
    if (!res || res->size() != 1)
        return entity;
    auto pos = entity.assign<Position>();
    res->getInt("x", pos->x_);
    res->getInt("y", pos->y_);
    res->getInt("map", pos->map_);
    res->getInt("revive_map", pos->spawn_);
    auto basic = entity.assign<BasicInfo>();
    res->getString("name", basic->name_);
    res->getInt("exp", basic->xp_);
    res->getInt("level", basic->level_);
    basic->id_ = charId;
    auto stats = entity.assign<Stats>();
    res->getInt("max_hp", stats->maxHp_);
    res->getInt("max_mp", stats->maxMp_);
    res->getInt("str", stats->str_);
    res->getInt("dex", stats->dex_);
    res->getInt("int_", stats->int_);
    res->getInt("con", stats->con_);
    res->getInt("charm", stats->charm_);
    res->getInt("sense", stats->sense_);
    auto advanced = entity.assign<AdvancedInfo>();
    res->getInt("zuly", advanced->zuly_);
    res->getInt("current_hp", advanced->hp_);
    res->getInt("current_mp", advanced->mp_);
    auto graphics = entity.assign<CharacterGraphics>();
    res->getInt("face", graphics->face_);
    res->getInt("hair", graphics->hair_);
    res->getInt("race", graphics->race_);
    auto info = entity.assign<CharacterInfo>();
    res->getInt("job", info->job_);
    res->getInt("stone", info->stone_);
    res->getInt("stat_points", info->statPoints_);
    res->getInt("skill_points", info->skillPoints_);
    res->getInt("penalty_exp", info->penaltyXp_);
    res->getInt("delete_date", info->deleteDate_);
    info->platinium_ = platinium;
    res->getInt("factionid", info->factionId_);
    res->getInt("faction_rank", info->factionRank_);
    res->getInt("faction_fame1", info->factionFame_[0]);
    res->getInt("faction_fame2", info->factionFame_[1]);
    res->getInt("faction_fame3", info->factionFame_[2]);
    res->getInt("faction_points1", info->factionPoints_[0]);
    res->getInt("faction_points2", info->factionPoints_[1]);
    res->getInt("faction_points3", info->factionPoints_[2]);
    res->getInt("clanid", info->guildId_);
    res->getInt("clan_contribution", info->guildContribution_);
    res->getInt("clan_rank", info->guildRank_);
    res->getInt("pk_flag", info->pkFlag_);
    res->getInt("stamina", info->stamina_);
    // TODO : write the pat initialization code
    auto skills = entity.assign<Skills>();
    auto sks = database.QStore(fmt::format("SELECT id, level from skill where char_id = {};", charId));
    if (sks) {
        size_t i = 0;
        for (auto &sk : *sks) {
            if (i >= 120)
                break;
            sk->getInt("id", skills->skills_[i].id_);
            sk->getInt("level", skills->skills_[i].level_);
            ++i;
        }
    }
    // TODO : write the hotbar table and loading code
    entity.assign<Hotbar>();
    auto equipped = entity.assign<EquippedItems>();
    res = database.QStore(fmt::format("CALL GetEquipped({});", charId));
    if (!res || !res->size())
        return entity;
    for (auto &it : *res) {
        size_t slot;
        uint16_t id, gemOpt;
        uint8_t refine;
        it->getInt("slot", slot);
        it->getInt("itemid", id);
        it->getInt("gem_opt", gemOpt);
        it->getInt("socket", equipped->items_[slot].hasSocket_);
        it->getInt("refine", refine);
        equipped->items_[slot].id_ = id;
        equipped->items_[slot].gemOpt_ = gemOpt;
        equipped->items_[slot].refine_ = refine;
    }
    entity.assign<StatusEffects>();
    return entity;
}
