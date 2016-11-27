#ifndef __ISERIALIZE_H__
#define __ISERIALIZE_H__

#include "crosepacket.h"
#include "idatabase.h"

namespace RoseCommon {

class ISerialize {
	public:
		virtual ~ISerialize() {}

	protected:
        virtual uint32_t getVisible() const = 0;
        virtual uint16_t getHeader() const = 0;
        virtual uint32_t getData() const = 0;

        virtual void saveToDatabase() const = 0;
        virtual void loadFromDatabase(Core::IRow&) = 0;
};

}

#endif
