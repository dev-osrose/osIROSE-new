#include "srv_removeobject.h"

namespace RoseCommon {

SrvRemoveObject::SrvRemoveObject() : CRosePacket(ePacketType::PAKWC_REMOVE_OBJECT) {
}

SrvRemoveObject::SrvRemoveObject(Entity entity) : CRosePacket(ePacketType::PAKWC_REMOVE_OBJECT), entity_(entity) {
}

Entity &SrvRemoveObject::entity() {
	return entity_;
}

void SrvRemoveObject::pack() {
	auto basicInfo = entity_.component<BasicInfo>();

	*this << basicInfo->id_;

}

}