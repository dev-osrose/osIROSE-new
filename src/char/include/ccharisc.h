#ifndef __CCHARISC_H__
#define __CCHARISC_H__

#include "croseisc.h"

class CCharISC : public CRoseISC
{
public:
        CCharISC( );
        CCharISC( tcp::socket _sock );
protected:
        bool HandlePacket( uint8_t* _buffer );
};

#endif
