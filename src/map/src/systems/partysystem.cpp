#include "systems/partysystem.h"
#include "cmapclient.h"
#include "makevector.h"

using namespace Systems;
using namespace RoseCommon;

PartySystem::PartySystem(SystemManager &m) : System(m) {
    m.registerDispatcher(ePacketType::PAKCS_PARTY_REQ, &PartySystem::processPartyReq);
    m.registerDispatcher(ePacketType::PAKCS_PARTY_REPLY, &PartySystem::processPartyReply);
    m.getEntityManager().on_component_removed<Party>([this](Entity entity, Party* Cparty) {
            this->logger_->trace("deleting the party for client {}", getId(entity));
            auto party = Cparty->party_;
            if (!party)
                return;
            if (!party->removeMember(entity))
                return;
            Entity starter = entity;
            if (Cparty->isKicked_)
                starter = party->leader_;
            if (auto socket = getClient(entity))
                socket->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::DESTROY, starter));
            if (party->members_.size() == 1)
                party->members_[0].remove<Party>();
            for (auto &it : party->members_) {
                if (auto socket = getClient(it))
                    socket->Send(*makePacket<ePacketType::PAKWC_PARTY_MEMBER>(party->options_, true, Core::make_vector(entity)));
            }
            });
    // TODO : use on_component_assign for new members?
}

void PartySystem::update(EntityManager &es, double) {
    for (Entity entity : es.entities_with_components<BasicInfo, Party>()) {
        Party *party = entity.component<Party>();
        if (!party->party_) {
            BasicInfo *info = entity.component<BasicInfo>();
            logger_->trace("Client {} has the Party component but no affiliated party. {}", info->id_, party->isRequested_);
            if (!party->isRequested_)
                entity.remove<Party>();
        }
    }
}

void PartySystem::addPartyMember(Entity leader, Entity newMember) {
    auto Cparty = leader.component<Party>();
    if (!Cparty)
        return;
    if (newMember.component<Party>())
        return;
    std::shared_ptr<PartyBase> party = Cparty->party_;
    if (!party)
        party = Cparty->party_ = std::make_shared<PartyBase>(leader);
    newMember.assign<Party>(party);
    for (auto &it : party->members_) {
        if (auto socket = getClient(it))
            socket->Send(*makePacket<ePacketType::PAKWC_PARTY_MEMBER>(party->options_, false, Core::make_vector(newMember)));
    }
    if (auto socket = getClient(newMember))
        socket->Send(*makePacket<ePacketType::PAKWC_PARTY_MEMBER>(party->options_, false, party->members_));
    party->addMember(newMember);
}

void PartySystem::changeLeader(Entity leader, Entity newLeader) {
    auto Cparty = leader.component<Party>();
    if (!Cparty)
        return;
    std::shared_ptr<PartyBase> party = Cparty->party_;
    if (!party)
        return;
    if (!party->changeLeader(newLeader))
        return;
    for (auto it : party->members_) {
        if (auto socket = getClient(it))
            socket->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::CHANGE_OWNER, newLeader));
    }
}

void PartySystem::processPartyReq(CMapClient& client, Entity entity, const CliPartyReq &packet) {
    logger_->trace("PartySystem::processPartyReq");
    Entity other;
    if (!(other = manager_.getEntity(packet.idXorTag())) || !getClient(other)) {
        logger_->debug("Client {} requested a party with the non existing char {}", getId(entity), packet.idXorTag());
        client.Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::NOT_FOUND, packet.idXorTag()));
        return;
    }
    Party* party = nullptr;
    switch (packet.request()) {
        case CliPartyReq::MAKE:
            if (entity.component<Party>() && entity.component<Party>()->party_) {
                logger_->warn("Client {} tried to create a party when already in a party", getId(entity));
                return;
            }
            party = entity.assign<Party>();
            party->isRequested_ = true;
            {
                if (auto socket = getClient(other))
                    socket->Send(*makePacket<ePacketType::PAKWC_PARTY_REQ>(static_cast<SrvPartyReq::Request>(packet.request()), entity));
            }
            return;
        case CliPartyReq::JOIN:
            if (!entity.component<Party>() || !entity.component<Party>()->party_) {
                logger_->warn("Client {} tried to execute an action on its party but doesn't have a party yet", getId(entity));
                return;
            }
            party = entity.component<Party>();
            party->isRequested_ = true;
            {
                if (auto socket = getClient(other))
                    socket->Send(*makePacket<ePacketType::PAKWC_PARTY_REQ>(static_cast<SrvPartyReq::Request>(packet.request()), entity));

            }
            return;
        case CliPartyReq::LEFT:
            if (!entity.component<Party>()) {
                logger_->warn("Client {} tried to leave the party but isn't in one", getId(entity));
                return;
            }
            entity.remove<Party>();
            return;
        case CliPartyReq::CHANGE_OWNER:
            if (!entity.component<Party>() || !entity.component<Party>()->party_) {
                logger_->warn("Client {} tried to give up ownership but isn't in a party", getId(entity));
                return;
            } else if (entity.component<Party>()->party_->leader_ != entity) {
                logger_->warn("Client {} tried to give up ownership but doesn't have it", getId(entity));
                return;
            }
            changeLeader(entity, other);
            return;
        case CliPartyReq::KICK:
            if (!entity.component<Party>() || !entity.component<Party>()->party_) {
                logger_->warn("Client {} tried to kick a member party but isn't in one", getId(entity));
                return;
            } else if (!entity.component<Party>()->party_->isMember(other)) {
                logger_->warn("Client {} tried to kick a member that isn't in its party", getId(entity));
                return;
            }
            other.component<Party>()->isKicked_ = true;
            other.remove<Party>();
            return;
        default:
            logger_->warn("Client {} sent a non valid request code {}", getId(entity), packet.request());
    }
}

void PartySystem::processPartyReply(CMapClient&, Entity entity, const RoseCommon::CliPartyReply &packet) {
    logger_->trace("PartySystem::processPartyRequest");
    Entity other;
    if (!(other = manager_.getEntity(packet.idXorTag())) || !getClient(other)) {
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
            {
                if(auto socket = getClient(other))
                    socket->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(static_cast<SrvPartyReply::Reply>(packet.reply()), entity));
            }
            return;
        // if the guy asked accepts, we send it to the guy asking and we update the party
        case CliPartyReply::ACCEPT_MAKE:
        case CliPartyReply::ACCEPT_JOIN:
            other.component<Party>()->isRequested_ = false;
            {
                if (auto socket = getClient(other))
                    socket->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(static_cast<SrvPartyReply::Reply>(packet.reply()), entity));

            }
            addPartyMember(other, entity);
            return;
        default:
            logger_->warn("Client {} sent a party reply with reply {}", getId(entity), (int)packet.reply());
    }
}
