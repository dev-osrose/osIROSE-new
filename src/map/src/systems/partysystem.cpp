#include "systems/partysystem.h"
#include "cmapclient.h"

using namespace Systems;
using namespace RoseCommon;

PartySystem::PartySystem(SystemManager &m) : System(m) {
    m.registerDispatcher(ePacketType::PAKCS_PARTY_REQ, &PartySystem::processPartyReq);
    m.registerDispatcher(ePacketType::PAKCS_PARTY_REPLY, &PartySystem::processPartyReply);
    m.getEntityManager().on_component_removed<Party>([this](Entity entity, Component<Party> party) {
            this->removePartyMember(party->party_, entity);
            });
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
            logger_->trace("client {}", it.component<BasicInfo>()->id_);
    }
}

PartySystem::addToPartyResult PartySystem::addToParty(Entity member, Entity newMember) {
    if (!member || !newMember)
        return PartySystem::ERROR;
    if (newMember.component<Party>())
        return PartySystem::ERROR;
    std::shared_ptr<PartyBase> party;
    bool isNew = false;
    if (!member.component<Party>()) {
        party = member.assign<Party>(std::make_shared<PartyBase>())->party_;
        party->members_.push_back(member);
        party->leader_ = member;
        isNew = true;
    } else {
        party = member.component<Party>()->party_;
        if (!party) {
            party = std::make_shared<PartyBase>();
            member.component<Party>()->party_ = party;
            party->members_.push_back(member);
            party->leader_ = member;
            isNew = true;
        }
    }
    if (party->members_.size() >= party->capacity_)
        return PartySystem::FULL;
    if (std::abs(newMember.component<BasicInfo>()->level_ - party->leader_.component<BasicInfo>()->level_) > party->range_)
        return PartySystem::INVALID_LEVEL;
    newMember.assign<Party>(party);
    party->members_.push_back(newMember);
    return isNew ? PartySystem::CREATED : PartySystem::ADDED;
}

void PartySystem::addPartyMember(Entity currentMember, Entity newMember) {
    auto res = addToParty(currentMember, newMember);
    CMapClient *currentMemberClient = currentMember.component<SocketConnector>()->client_;
    CMapClient *newMemberClient = newMember.component<SocketConnector>()->client_;
    switch (res) {
        case PartySystem::ERROR: // That should never happen, but we never know
        default:
            logger_->warn("There was an error while partying client {} and client {}", newMember.component<BasicInfo>()->id_, currentMember.component<BasicInfo>()->id_);
            newMemberClient->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::NOT_FOUND, currentMember));
            currentMemberClient->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::NOT_FOUND, newMember));
            return;
        case PartySystem::FULL:
        case PartySystem::INVALID_LEVEL:
        {
            auto tmp = res == PartySystem::FULL ? SrvPartyReply::FULL : SrvPartyReply::INVALID_LEVEL;
            newMemberClient->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(tmp, currentMember));
            return;
        }
        case PartySystem::CREATED:
            currentMemberClient->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::ACCEPT_MAKE, newMember));
        case PartySystem::ADDED:
        {
            newMemberClient->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::ACCEPT_JOIN, currentMember));
            // send the new character to the current members
            auto party = currentMember.component<Party>()->party_;
            for (auto it : party->members_) {
                if (it == newMember)
                    continue;
                CMapClient *socket;
                if (it.component<SocketConnector>() && (socket = it.component<SocketConnector>()->client_)) {
                    auto list = { newMember };
                    socket->Send(*makePacket<ePacketType::PAKWC_PARTY_MEMBER>(party->options_, false, list));
                }
            }
            // now we send all of the members to the new member
            newMemberClient->Send(*makePacket<ePacketType::PAKWC_PARTY_MEMBER>(party->options_, false, party->members_));
        }
    }
}

void PartySystem::removePartyMember(std::shared_ptr<PartyBase> party, Entity memberToRemove) {
    if (!memberToRemove || !party || party->members_.size() == 1) // if there's only one guy in the party there's nothing to do (it was done previously)
        return;
    bool wasInParty = false;
    bool wasLeader = memberToRemove == party->leader_;
    for (auto it = party->members_.cbegin(); it != party->members_.cend(); ++it)
        if (*it == memberToRemove) {
            wasInParty = true;
            party->members_.erase(it);
            break;
        }
    if (!wasInParty)
        return;
    if (party->members_.size() == 1) { // there's only one guy left on the party, we remove it
        auto client = party->members_[0].component<SocketConnector>()->client_;
        client->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::DESTROY, party->members_[0]));
        party->members_[0].remove<Party>();
        return;
    }
    if (wasLeader)
        party->leader_ = party->members_[0];
    for (auto it : party->members_) {
        CMapClient *socket;
        if (it.component<SocketConnector>() && (socket = it.component<SocketConnector>()->client_))
            socket->Send(*makePacket<ePacketType::PAKWC_PARTY_MEMBER>(party->options_, true, std::vector<Entity>{ memberToRemove, party->leader_ }));
    }
}

void PartySystem::changePartyLeader(std::shared_ptr<PartyBase> party, Entity newLeader) {
    if (!newLeader || !party)
        return;
    bool found = false;
    for (auto it : party->members_)
        if (it == newLeader) {
            found = true;
            break;
        }
    if (!found)
        return;
    bool hasChanged = newLeader == party->leader_;
    if (!hasChanged)
        return;
    party->leader_ = newLeader;
    for (auto it : party->members_) {
        CMapClient *socket;
        if (it.component<SocketConnector>() && (socket = it.component<SocketConnector>()->client_))
            socket->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::CHANGE_OWNER, newLeader));
    }
}

void PartySystem::kickPartyMember(std::shared_ptr<PartyBase> party, Entity member) {
    if (!member || !party)
        return;
    if (party->members_.size() == 1) {
        member.remove<Party>();
        return;
    }
    bool found = false;
    for (auto it = party->members_.begin(); it != party->members_.end(); ++it)
        if (*it == member) {
            found = true;
            party->members_.erase(it);
            break;
        }
    if (!found)
        return;
    //if (member.component<SocketConnector>() && member.component<SocketConnector>()->client_)
    //    member.component<SocketConnector>()->client_->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::KICK, member));
    //member.component<Party>()->party_.reset();
    member.remove<Party>();
    //for (auto it : party->members_) {
    //    CMapClient *socket;
    //    if (it && it.component<SocketConnector>() && (socket = it.component<SocketConnector>()->client_))
    //        socket->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::KICK, member));
    //}
    //if (party->members_.size() == 1)
    //    party->members_[0].remove<Party>();
}

void PartySystem::processPartyReq(CMapClient *client, Entity entity, const CliPartyReq &packet) {
    logger_->trace("PartySystem::processPartyReq");
    Entity other;
    if (!(other = manager_.getEntity(packet.idXorTag())) || !other.component<SocketConnector>() || !other.component<SocketConnector>()->client_) {
        logger_->debug("Client {} requested a party with the non existing char {}", entity.component<BasicInfo>()->id_, packet.idXorTag());
        client->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(SrvPartyReply::NOT_FOUND, packet.idXorTag()));
        return;
    }
    bool isMake = false;
    switch (packet.request()) {
        case CliPartyReq::MAKE:
            if (entity.component<Party>()) {
                logger_->warn("Client {} tried to create a party when already in a party", entity.component<BasicInfo>()->id_);
                return;
            }
            isMake = true;
        case CliPartyReq::JOIN:
            if (!isMake && !entity.component<Party>()) {
                logger_->warn("Client {} tried to execute an action on its party but doesn't have a party yet", entity.component<BasicInfo>()->id_);
                return;
            }
            if (isMake)
                entity.assign<Party>()->isRequested_ = true;
            else
                entity.component<Party>()->isRequested_ = true;
            {
                CMapClient *otherClient = other.component<SocketConnector>()->client_;
                otherClient->Send(*makePacket<ePacketType::PAKWC_PARTY_REQ>(static_cast<SrvPartyReq::Request>(packet.request()), entity));
            }
            return;
        case CliPartyReq::LEFT:
            if (!entity.component<Party>())
                return;
            entity.remove<Party>();
            return;
        case CliPartyReq::CHANGE_OWNER:
            if (!entity.component<Party>())
                return;
            changePartyLeader(entity.component<Party>()->party_, other);
            return;
        case CliPartyReq::KICK:
            if (!entity.component<Party>())
                return;
            kickPartyMember(entity.component<Party>()->party_, other);
            return;
        default:
            logger_->warn("Client {} sent a non valid request code {}", entity.component<BasicInfo>()->id_, packet.request());
    }
}

void PartySystem::processPartyReply(CMapClient*, Entity entity, const RoseCommon::CliPartyReply &packet) {
    logger_->trace("PartySystem::processPartyRequest");
    Entity other;
    if (!(other = manager_.getEntity(packet.idXorTag())) || !other.component<SocketConnector>() || !other.component<SocketConnector>()->client_) {
        logger_->warn("Client {} replied to a party request of the non existing char {}", entity.component<BasicInfo>()->id_, packet.idXorTag());
        return;
    }
    if (!other.component<Party>() || !other.component<Party>()->isRequested_) {
        logger_->warn("Client {} replied to a non existing party request", entity.component<BasicInfo>()->id_);
        return;
    }
    CMapClient *otherClient = other.component<SocketConnector>()->client_;
    switch (packet.reply()) {
        // if the guy asked is busy or declined the invitation, we send it to the guy asking
        case CliPartyReply::BUSY:
        case CliPartyReply::REJECT:
            other.component<Party>()->isRequested_ = false;
            if (!other.component<Party>()->party_)
                other.remove<Party>();
            otherClient->Send(*makePacket<ePacketType::PAKWC_PARTY_REPLY>(static_cast<SrvPartyReply::Reply>(packet.reply()), entity));
            return;
        // if the guy asked accepts, we send it to the guy asking and we update the party
        case CliPartyReply::ACCEPT_MAKE:
        case CliPartyReply::ACCEPT_JOIN:
            other.component<Party>()->isRequested_ = false;
            addPartyMember(other, entity);
            return;
        default:
            logger_->warn("Client {} sent a party reply with reply {}", entity.component<BasicInfo>()->id_, (int)packet.reply());
    }
}
