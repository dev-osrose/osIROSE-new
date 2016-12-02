#include "systems/partysystem.h"
#include "cmapclient.h"

using namespace Systems;
using namespace RoseCommon;

PartySystem::PartySystem(SystemManager &m) : System(m) {
    m.registerDispatcher(ePacketType::PAKCS_PARTY_REQ, &PartySystem::processPartyReq);
    m.registerDispatcher(ePacketType::PAKCS_PARTY_REPLY, &PartySystem::processPartyReply);
}

void PartySystem::update(EntityManager&, double) {}

std::pair<bool, bool> PartySystem::addToParty(Entity member, Entity newMember) {
    if (!member || !newMember)
        return std::make_pair(false, false);
    if (newMember.component<Party>())
        return std::make_pair(false, false);
    std::shared_ptr<PartyBase> party;
    bool isNew = false;
    if (!member.component<Party>()) {
        party = member.assign<Party>(std::make_shared<PartyBase>())->party_;
        party->members_.push_back(member);
        party->capacity_ = 5; // FIXME : put that in a #define or something
        party->maxLevel_ = member.component<BasicInfo>()->level_ = 5; // FIXME : put that in a #define or something
        isNew = true;
    } else
        party = member.component<Party>()->party_;
    if (party->members_.size() >= party->capacity_)
        return std::make_pair(false, false);
    if (newMember.component<BasicInfo>()->level_ > party->maxLevel_)
        return std::make_pair(false, false);
    newMember.assign<Party>(party);
    party->members_.push_back(newMember);
    return std::make_pair(true, isNew);
}

void PartySystem::processPartyReq(CMapClient*, Entity entity, const CliPartyReq &packet) {
    logger_->trace("PartySystem::processPartyReq");
    Entity other;
    if (!(other = manager_.getEntity(packet.idXorTag())) || !other.component<SocketConnector>() || !other.component<SocketConnector>()->client_) {
        logger_->warn("Client {} requested a party with the non existing char {}", entity.component<BasicInfo>()->id_, packet.idXorTag());
        return;
    }
    CMapClient *otherClient = other.component<SocketConnector>()->client_;
    otherClient->Send(*makePacket<ePacketType::PAKWC_PARTY_REQ>(static_cast<SrvPartyReq::Request>(packet.request()), entity));
}

void PartySystem::processPartyReply(CMapClient *, Entity , const RoseCommon::CliPartyReply &) {

}
