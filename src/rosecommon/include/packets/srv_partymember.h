#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <vector>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_MEMBER, SrvPartyMember)
class SrvPartyMember : public CRosePacket {
	public:
		SrvPartyMember();
		SrvPartyMember(uint8_t rules, bool isDelete, const std::vector<Entity> &list);
        SrvPartyMember(uint8_t rules, bool isDelete, Entity entity);

		virtual ~SrvPartyMember() = default;

		uint8_t rules() const;
		bool isDelete() const;
        const std::vector<Entity> &list() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t rules_;
		bool isDelete_;
        std::vector<Entity> list_;
};

}
