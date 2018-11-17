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
		SrvQuestData(const Quests& quests, const Wishlist& wishlist);
	public:

		virtual ~SrvQuestData() = default;

		const Quests& quests() const;
		SrvQuestData& set_quests(const Quests&);
		const Wishlist& wishlist() const;
		SrvQuestData& set_wishlist(const Wishlist&);

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