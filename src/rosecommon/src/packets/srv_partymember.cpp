#include "srv_partymember.h"

namespace RoseCommon {

SrvPartyMember::SrvPartyMember() : CRosePacket(ePacketType::PAKWC_PARTY_MEMBER) {}

SrvPartyMember::SrvPartyMember(uint8_t rules, bool isDelete, const std::vector<Entity> &list) : CRosePacket(ePacketType::PAKWC_PARTY_MEMBER), rules_(rules), isDelete_(isDelete), list_(list) {}

uint8_t SrvPartyMember::rules() const {
	return rules_;
}

bool SrvPartyMember::isDelete() const {
	return isDelete_;
}

const std::vector<Entity> &SrvPartyMember::list() const {
	return list_;
}


void SrvPartyMember::pack() {
	*this << rules_;
    if (isDelete_) {
        *this << (int8_t)-1;
        *this << list_[0].component<BasicInfo>()->tag_; // leaving
        *this << list_[1].component<BasicInfo>()->tag_; // leader
        return;
    }

    *this << (uint8_t)list_.size();
    for (auto it : list_) {
        auto basic = it.component<BasicInfo>();
        auto advanced = it.component<AdvancedInfo>();
        auto stats = it.component<Stats>();
        auto character = it.component<CharacterInfo>();
        *this << basic->tag_;
        *this << basic->id_;
        *this << (int16_t)stats->maxHp_;
        *this << (int16_t)advanced->hp_;
        *this << character->statusFlag_;
        *this << stats->con_;
        *this << (uint8_t)0; // TODO : RECOVERY HP
        *this << (uint8_t)0; // TODO : RECOVERY MP
        *this << character->stamina_;
        *this << basic->name_;
    }
}

}
