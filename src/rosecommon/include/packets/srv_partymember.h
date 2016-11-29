#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <initializer_list>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_MEMBER, SrvPartyMember)
class SrvPartyMember : public CRosePacket {
	public:
		SrvPartyMember();
		SrvPartyMember(uint8_t rules, bool isDelete, std::initializer_list<Entity> list);

		virtual ~SrvPartyMember() = default;

		uint8_t rules() const;
		bool isDelete() const;
		std::initializer_list<Entity> list() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t rules_;
		bool isDelete_;
		std::initializer_list<Entity> list_;
};

}