#include "srv_dropitem.h"
#include "throwassert.h"

namespace RoseCommon {

SrvDropItem::SrvDropItem() : CRosePacket(ePacketType::PAKWC_DROP_ITEM) {}

SrvDropItem::SrvDropItem(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKWC_DROP_ITEM, "Not the right packet: " << to_underlying(type()));
	auto position = entity_.component<Position>();
	auto item = entity_.component<RoseCommon::Item>();
	auto basicInfo = entity_.component<BasicInfo>();

	*this >> position->x_;
	*this >> position->y_;
	//TODO : implement roseCommon->Item_
    (void)item;
	*this >> basicInfo->id_;
	*this >> basicInfo->ownerId_;

    uint16_t remainTime;
    *this >> remainTime;

}

SrvDropItem::SrvDropItem(Entity entity) : CRosePacket(ePacketType::PAKWC_DROP_ITEM), entity_(entity) {}

Entity SrvDropItem::entity() const {
	return entity_;
}


void SrvDropItem::pack() {
	auto position = entity_.component<Position>();
	auto item = entity_.component<RoseCommon::Item>();
	auto basicInfo = entity_.component<BasicInfo>();

	*this << position->x_;
	*this << position->y_;
    *this << item->getHeader();
    *this << item->getData();
	*this << basicInfo->id_;
	*this << basicInfo->ownerId_;
    *this << (uint16_t)5; // TODO: encode the remaining time somehow

}

}
