#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_DROP_ITEM, CliDropItem)
class CliDropItem : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> init;
	public:
		CliDropItem();
		CliDropItem(uint8_t buffer[MAX_PACKET_SIZE]);
		CliDropItem(uint8_t item, uint32_t quantity);

		virtual ~CliDropItem() = default;

		uint8_t item() const;
		uint32_t quantity() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t item_;
		uint32_t quantity_;
};

}