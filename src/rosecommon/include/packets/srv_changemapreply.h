#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_CHANGE_MAP_REPLY, SrvChangeMapReply)
REGISTER_SEND_PACKET(ePacketType::PAKWC_CHANGE_MAP_REPLY, SrvChangeMapReply)
class SrvChangeMapReply : public CRosePacket {
	public:
		SrvChangeMapReply();
		SrvChangeMapReply(CRoseReader reader);
	private:
		SrvChangeMapReply(uint16_t objectIndex, uint16_t currentHp, uint16_t currentMp, uint16_t currentExp, uint16_t penalizeExp, uint16_t worldTime, uint16_t teamNumber);
	public:

		virtual ~SrvChangeMapReply() = default;

		uint16_t objectIndex() const;
		SrvChangeMapReply& set_objectIndex(uint16_t);
		uint16_t currentHp() const;
		SrvChangeMapReply& set_currentHp(uint16_t);
		uint16_t currentMp() const;
		SrvChangeMapReply& set_currentMp(uint16_t);
		uint16_t currentExp() const;
		SrvChangeMapReply& set_currentExp(uint16_t);
		uint16_t penalizeExp() const;
		SrvChangeMapReply& set_penalizeExp(uint16_t);
		uint16_t craftRate() const;
		SrvChangeMapReply& set_craftRate(uint16_t);
		uint16_t updateTime() const;
		SrvChangeMapReply& set_updateTime(uint16_t);
		uint16_t worldRate() const;
		SrvChangeMapReply& set_worldRate(uint16_t);
		uint8_t townRate() const;
		SrvChangeMapReply& set_townRate(uint8_t);
		const uint8_t* itemRate() const;
		uint8_t itemRate(size_t index) const;
		SrvChangeMapReply& set_itemRate(const uint8_t[MAX_SELL_TYPE]);
		SrvChangeMapReply& set_itemRate(uint8_t, size_t index);
		uint32_t flags() const;
		SrvChangeMapReply& set_flags(uint32_t);
		uint16_t worldTime() const;
		SrvChangeMapReply& set_worldTime(uint16_t);
		uint16_t teamNumber() const;
		SrvChangeMapReply& set_teamNumber(uint16_t);

		static SrvChangeMapReply create(uint16_t objectIndex, uint16_t currentHp, uint16_t currentMp, uint16_t currentExp, uint16_t penalizeExp, uint16_t worldTime, uint16_t teamNumber);
		static SrvChangeMapReply create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t objectIndex_;
		uint16_t currentHp_;
		uint16_t currentMp_;
		uint16_t currentExp_;
		uint16_t penalizeExp_;
		uint16_t craftRate_= 0;
		uint16_t updateTime_= 0;
		uint16_t worldRate_= 0;
		uint8_t townRate_= 0;
		uint8_t itemRate_[MAX_SELL_TYPE]= {0};
		uint32_t flags_= 0;
		uint16_t worldTime_;
		uint16_t teamNumber_;
};

}