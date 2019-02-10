#include "party.h"
#include "entity_system.h"
#include "logconsole.h"

#include "components/basic_info.h"
#include "components/party.h"
#include "components/computed_values.h"
#include "components/life.h"
#include "components/stamina.h"
#include "components/stats.h"

#include "srv_party_reply.h"
#include "srv_party_member.h"

using namespace RoseCommon::Packet;

namespace Party {
void add_member(EntitySystem& entitySystem, RoseCommon::Entity leader, RoseCommon::Entity member) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Party::add_member");
    auto& p = entitySystem.get_component<Component::Party>(leader);
    if (entitySystem.has_component<Component::Party>(member)) {
        return;
    }
    auto party = p.party;
    if (!party) {
        party = p.party = std::make_shared<PartyBase>(leader);
    }
    auto pp = entitySystem.add_component<Component::Party>(member);
    pp.party = party;
    pp.isKicked = false;
    pp.isRequested = false;
    RoseCommon::PartyData data;
    RoseCommon::PartyData::Member m;
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(member);
    const auto& life = entitySystem.get_component<Component::Life>(member);
    const auto& stats = entitySystem.get_component<Component::Stats>(member);
    const auto& computedValues = entitySystem.get_component<Component::ComputedValues>(member);
    const auto& stamina = entitySystem.get_component<Component::Stamina>(member);
    m.set_tag(basicInfo.tag);
    m.set_id(basicInfo.id);
    m.set_maxHp(life.max_hp);
    m.set_hp(life.hp);
    m.set_statusFlag(computedValues.status_flag);
    m.set_con(stats.con);
    m.set_recoveryHp(0);
    m.set_recoveryMp(0);
    m.set_stamina(stamina.stamina);
    m.set_name(basicInfo.name);
    data.add_member(m);
    for (const auto& it : party->members) {
        entitySystem.send_to(it, SrvPartyMember::create(party->options, data));
    }
    RoseCommon::PartyData ppp;
    for (const auto& it : party->members) {
        RoseCommon::PartyData::Member m;
        const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(it);
        const auto& life = entitySystem.get_component<Component::Life>(it);
        const auto& stats = entitySystem.get_component<Component::Stats>(it);
        const auto& computedValues = entitySystem.get_component<Component::ComputedValues>(it);
        const auto& stamina = entitySystem.get_component<Component::Stamina>(it);
        m.set_tag(basicInfo.tag);
        m.set_id(basicInfo.id);
        m.set_maxHp(life.max_hp);
        m.set_hp(life.hp);
        m.set_statusFlag(computedValues.status_flag);
        m.set_con(stats.con);
        m.set_recoveryHp(0);
        m.set_recoveryMp(0);
        m.set_stamina(stamina.stamina);
        m.set_name(basicInfo.name);
        data.add_member(m);
    }
    entitySystem.send_to(member, SrvPartyMember::create(party->options, ppp));
    party->add_member(member);
}

void remove_member(EntitySystem& entitySystem, RoseCommon::Entity member) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Party::remove_member");
    auto& p = entitySystem.get_component<Component::Party>(member);
    if (!p->party) { // no party
        entitySystem.remove_component<Component::Party>(member);
        return;
    }
    auto* party = p.party;
    if (!party->remove_member(member)) {
        return;
    }
    const RoseCommon::Entity initiator = p->isKicked ? party->leader : member;
    const uint32_t tag = entitySystem.get_component<Component::BasicInfo>(initiator).tag;
    entitySystem.send_to(member, SrvPartyReply::create(SrvPartyReply::DESTROY, tag));
    if (party->members.size() == 1) {
        entitySystem.remove_component<Component::Party>(party->members[0]);
        return;
    }
    const uint32_t leaver = entitySystem.get_component<Component::BasicInfo>(member).tag;
    const uint32_t leader = entitySystem.get_component<Component::BasicInfo>(party->leader).tag;
    RoseCommon::PartyData data;
    data.set_leaver(leaver);
    data.set_leader(leader);
    for (const auto& it : party->members) {
        entitySystem.send_to(it, SrvPartyMember::create(party->options, data));
    }
}

void change_leader(EntitySystem& entitySystem, RoseCommon::Entity current_leader, RoseCommon::Entity new_leader) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Party::change_leader");
    auto& p = entitySystem.get_component<Component::Party>(current_leader);
    if (!p->party) { // no party
        entitySystem.remove_component<Component::Party>(current_leader);
        return;
    }
    if (!party->change_leader(new_leader)) {
        return;
    }
    const uint32_t tag = entitySystem.get_component<Component::BasicInfo>(new_leader).tag;
    for (const auto& it : party->members) {
        entitySystem.send_to(it, SrvPartyReply::create(SrvPartyReply::CHANGE_OWNER, tag));
    }
}

void process_party_request(EntitySystem& entitySystem, RoseCommon::Entity, const RoseCommon::Packet::CliPartyReq& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Party::process_party_request");
}

void process_party_reply(EntitySystem& entitySystem, RoseCommon::Entity, const RoseCommon::Packet::CliPartyReply& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Party::process_party_reply");
}
}
