#include "srv_partymember.h"

namespace RoseCommon {

SrvPartyMember::SrvPartyMember() : CRosePacket(ePacketType::PAKWC_PARTY_MEMBER) {}

SrvPartyMember::SrvPartyMember(uint8_t rules, bool isDelete, std::initializer_list<Entity> list) : CRosePacket(ePacketType::PAKWC_PARTY_MEMBER), rules_(rules), isDelete_(isDelete), list_(list) {}

uint8_t SrvPartyMember::rules() const {
	return rules_;
}

bool SrvPartyMember::isDelete() const {
	return isDelete_;
}

std::initializer_list<Entity> SrvPartyMember::list() const {
	return list_;
}


void SrvPartyMember::pack() {
	*this << rules_;
    if (isDelete_) {
        *this << (int8_t)-1;
        for (auto it : list_) // leaving, leader
            *this << it.component<BasicInfo>()->tag_;
        return;
    }

    for (auto it : list_) {
        auto basic = it.component<BasicInfo>();
        auto advanced = it.component<AdvancedInfo>();
        auto stats = it.component<Stats>();
        auto character = it.component<CharacterInfo>();
        *this << basic->tag_;
        *this << basic->id_;
        *this << stats->maxHp_;
        *this << advanced->hp_;
        *this << character->statusFlag_;
        *this << stats->con_;
        *this << (uint8_t)0; // TODO : RECOVERY HP
        *this << (uint8_t)0; // TODO : RECOVERY MP
        *this << character->stamina_;
        *this << basic->name_;
    }
}

}
