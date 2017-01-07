#include "systems/partysystem.h"
#include "cmapclient.h"

using namespace Systems;
using namespace RoseCommon;

PartySystem::PartySystem(SystemManager &m) : System(m) {
    m.registerDispatcher(ePacketType::PAKCS_PARTY_REQ, &PartySystem::processPartyReq);
    m.registerDispatcher(ePacketType::PAKCS_PARTY_REPLY, &PartySystem::processPartyReply);
    m.getEntityManager().on_component_removed<Party>([this](Entity entity, Component<Party> Cparty) {
            auto party = Cparty->party_;
            party->removeMember(entity);
            for (auto &it : party->members_)
                getClient(it)->Send(*makePacket<ePacketType::PAKWC_PARTY_MEMBER>(party->options_, true, entity));
            });
    // TODO : use on_component_assign for new members?
}

void PartySystem::update(EntityManager &es, double) {
    Component<Party> party;
    Component<BasicInfo> info;
    for (Entity entity : es.entities_with_components(info, party)) {
        (void)entity;
        if (!party->party_) {
            logger_->trace("Client {} has the Party component but no affiliated party. {}", info->id_, party->isRequested_);
        }
        logger_->trace("Client {} is in a party", info->id_);
        logger_->trace("Party leader is {}, party members are", party->party_->leader_);
        for (auto it : party->party_->members_)
            logger_->trace("client {}", getId(it));
    }
}

void PartySystem::addPartyMember(Entity leader, Entity newMember) {
    auto Cparty = leader.component<Party>();
    if (!Cparty)
        return;
    std::shared_ptr<PartyBase> party = Cparty->party_;
    if (!party)
        party = Cparty->party_ = std::make_shared<PartyBase>(leader, newMember);
    if (newMember.component<Party>())
        return;
    newMember.assign<Party>(party);
    for (auto &it : party->members_) {
        getClient(it)->Send(*makePacket<ePacketType::PAKWC_PARTY_MEMBER>(party->options_, false, newMember));
    }
    getClient(newMember)->Send(*makePacket<ePacketType::PAKWC_PARTY_MEMBER>(party->options_, false, party->members_));
    party->addMember(newMember);
}

void PartySystem::processPartyReq(CMapClient *client, Entity entity, const CliPartyReq &packet) {
    logger_->trace("PartySystem::processPartyReq");
    Entity other;
    if (!(other = manager_.getEntity(packet.idXorTag())) || !getClient(other)) {
        logger_->debug("Client {} requested a party with the non existing char {}", getId(entity), packet.idXorTag());
        client->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::NOT_FOUND, packet.idXorTag()));
        return;
    }
    Component<Party> party;
    switch (packet.request()) {
        case CliPartyReq::MAKE:
            if (entity.component<Party>()) {
                logger_->warn("Client {} tried to create a party when already in a party", getId(entity));
                return;
            }
            party = entity.assign<Party>();
        case CliPartyReq::JOIN:
            if (!entity.component<Party>()) {
                logger_->warn("Client {} tried to execute an action on its party but doesn't have a party yet", getId(entity));
                return;
            }
            party = entity.component<Party>();
            party->isRequested_ = true;
            getClient(other)->Send(*makePacket<ePacketType::PAKWC_PARTY_REQ>(static_cast<SrvPartyReq::Request>(packet.request()), entity));
            return;
        case CliPartyReq::LEFT:
            if (!entity.component<Party>()) {
                logger_->warn("Client {} tried to leave the party but isn't in one", getId(entity));
                return;
            }
            entity.remove<Party>();
            return;
        case CliPartyReq::CHANGE_OWNER:
            if (!entity.component<Party>()) {
                logger_->warn("Client {} tried to give up ownership but isn't in a party", getId(entity));
                return;
            }
            // TODO : change leader
            return;
        case CliPartyReq::KICK:
            if (!entity.component<Party>())
                return;
            // TODO : kick member
            return;
        default:
            logger_->warn("Client {} sent a non valid request code {}", getId(entity), packet.request());
    }
}

void PartySystem::processPartyReply(CMapClient*, Entity entity, const RoseCommon::CliPartyReply &packet) {
    logger_->trace("PartySystem::processPartyRequest");
    Entity other;
    if (!(other = manager_.getEntity(packet.idXorTag())) || getClient(other)) {
        logger_->warn("Client {} replied to a party request of the non existing char {}", getId(entity), packet.idXorTag());
        return;
    }
    if (!other.component<Party>() || !other.component<Party>()->isRequested_) {
        logger_->warn("Client {} replied to a non existing party request", getId(entity));
        return;
    }
    switch (packet.reply()) {
        // if the guy asked is busy or declined the invitation, we send it to the guy asking
        case CliPartyReply::BUSY:
        case CliPartyReply::REJECT:
            other.component<Party>()->isRequested_ = false;
            if (!other.component<Party>()->party_)
                other.remove<Party>();
            getClient(other)->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(static_cast<SrvPartyReply::Reply>(packet.reply()), entity));
            return;
        // if the guy asked accepts, we send it to the guy asking and we update the party
        case CliPartyReply::ACCEPT_MAKE:
        case CliPartyReply::ACCEPT_JOIN:
            other.component<Party>()->isRequested_ = false;
            // TODO : add party member
            return;
        default:
            logger_->warn("Client {} sent a party reply with reply {}", getId(entity), (int)packet.reply());
    }
}
