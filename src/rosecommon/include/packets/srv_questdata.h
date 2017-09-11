#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_QUEST_DATA, SrvQuestData)
class SrvQuestData : public CRosePacket {
	public:
		SrvQuestData();
		SrvQuestData(uint8_t buffer[MAX_PACKET_SIZE]);
		SrvQuestData(Entity entity);

		virtual ~SrvQuestData() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}