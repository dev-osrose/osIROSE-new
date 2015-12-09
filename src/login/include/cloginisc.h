#ifndef _CLOGINISC_H_
#define _CLOGINISC_H_

#include "croseisc.h"

class CLoginISC : public CRoseISC
{
public:
        CLoginISC( );
        CLoginISC( tcp::socket _sock );
};

#endif

