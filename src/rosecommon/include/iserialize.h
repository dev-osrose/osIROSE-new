#ifndef __ISERIALIZE_H__
#define __ISERIALIZE_H__

#include "crosepacket.h"

namespace RoseCommon {

class CRosePacket;

class ISerialize {
	public:
		virtual ~ISerialize() {}

	protected:
		friend CRosePacket &operator<<(CRosePacket&, const ISerialize&);
		friend CRosePacket &operator>>(CRosePacket&, ISerialize&);

		virtual void serialize(CRosePacket &os) const = 0;
		virtual void deserialize(CRosePacket &os) = 0;
};
}

#endif
