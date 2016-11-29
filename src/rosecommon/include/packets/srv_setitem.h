#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <initializer_list>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_ITEM, SrvSetItem)
class SrvSetItem : public CRosePacket {
	public:
		SrvSetItem();
		SrvSetItem(Entity entity, std::initializer_list<uint8_t> list);

		virtual ~SrvSetItem() = default;

		Entity entity() const;
		std::initializer_list<uint8_t> list() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
		std::initializer_list<uint8_t> list_;
};

}