#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_QUEST_DATA, SrvQuestData)
REGISTER_SEND_PACKET(ePacketType::PAKWC_QUEST_DATA, SrvQuestData)
class SrvQuestData : public CRosePacket {
	public:
		SrvQuestData();
		SrvQuestData(CRoseReader reader);
	private:
		SrvQuestData(Quests quests, Wishlist wishlist);
	public:

		virtual ~SrvQuestData() = default;

		Quests quests() const;
		SrvQuestData& set_quests(Quests);
		Wishlist wishlist() const;
		SrvQuestData& set_wishlist(Wishlist);

		static SrvQuestData create(Entity entity);
		static SrvQuestData create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		Quests quests_;
		Wishlist wishlist_;
};

}