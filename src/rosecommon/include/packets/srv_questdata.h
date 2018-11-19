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
		static std::unique_ptr<SrvQuestData> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		Quests quests_;
		Wishlist wishlist_;
};

}