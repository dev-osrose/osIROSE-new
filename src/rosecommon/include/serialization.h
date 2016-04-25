#ifndef __SERIALIZATION_H__
#define __SERIALIZATION_H__


#include "crosepacket.h"

class ISerialization {
	public:
		virtual ~ISerialization() {}

	protected:
		friend CRosePacket;

		void serialize(CRosePacket &os) const = 0;
		void deserialize(CRosePacket &os) = 0;
};

#endif
