#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_GLOBAL_VARS, SrvGlobalVars)
REGISTER_SEND_PACKET(ePacketType::PAKWC_GLOBAL_VARS, SrvGlobalVars)
class SrvGlobalVars : public CRosePacket {
	public:
		SrvGlobalVars();
		SrvGlobalVars(CRoseReader reader);
	public:

		virtual ~SrvGlobalVars() = default;

		ServerData::Type type() const;
		SrvGlobalVars& set_type(ServerData::Type);
		uint32_t counter() const;
		SrvGlobalVars& set_counter(uint32_t);
		uint16_t popBase() const;
		SrvGlobalVars& set_popBase(uint16_t);
		uint16_t devBase() const;
		SrvGlobalVars& set_devBase(uint16_t);
		const uint16_t* consume() const;
		const uint16_t& consume(size_t index) const;
		SrvGlobalVars& set_consume(const uint16_t[MAX_SELL_TYPE]);
		SrvGlobalVars& set_consume(const uint16_t&, size_t index);
		uint16_t dev() const;
		SrvGlobalVars& set_dev(uint16_t);
		uint32_t pop() const;
		SrvGlobalVars& set_pop(uint32_t);
		const uint32_t* item() const;
		const uint32_t& item(size_t index) const;
		SrvGlobalVars& set_item(const uint32_t[MAX_SELL_TYPE]);
		SrvGlobalVars& set_item(const uint32_t&, size_t index);

		static SrvGlobalVars create();
		static SrvGlobalVars create(uint8_t *buffer);
		static std::unique_ptr<SrvGlobalVars> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		ServerData::Type type_= ServerData::Type::ECONOMY;
		uint32_t counter_= 0;
		uint16_t popBase_= 0;
		uint16_t devBase_= 0;
		uint16_t consume_[MAX_SELL_TYPE]= {0};
		uint16_t dev_= 0;
		uint32_t pop_= 0;
		uint32_t item_[MAX_SELL_TYPE]= {0};
};

}