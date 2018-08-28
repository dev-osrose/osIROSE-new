#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_HP_AND_MP, SrvSetHpAndMp)
class SrvSetHpAndMp : public CRosePacket {
	public:
		SrvSetHpAndMp();
		SrvSetHpAndMp(Entity entity);

		virtual ~SrvSetHpAndMp() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}