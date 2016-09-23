#ifndef __IOBJECT_H__
#define __IOBJECT_H__

#include "crosepacket.h"

namespace RoseCommon {

class CRosePacket;

/*!
 * \class IObject
 *
 * \brief The base class for the game objects
 *
 * This class is used to encapsulate the basic data that are needed for game processing logic
 *
 * \author Raven
 * \date AUG 2016
 */
class IObject {
	public:
		virtual ~IObject() {}
		
		virtual uint32_t GetObjId() const = 0;
		virtual bool IsNearby(const IObject* _otherClient) const = 0;
};
}
#endif