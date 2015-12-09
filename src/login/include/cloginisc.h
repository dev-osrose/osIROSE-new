#ifndef _CLOGINISC_H_
#define _CLOGINISC_H_

#include "croseisc.h"

class CLoginISC : public CRoseISC
{
public:
        CLoginISC( );
        CLoginISC( tcp::socket _sock );
protected:
	bool HandlePacket( uint8_t* _buffer );
};

#endif

