#include "cloginisc.h"

CLoginISC::CLoginISC() : CRoseISC()
{
}

CLoginISC::CLoginISC( tcp::socket _sock ) : CRoseISC( std::move( _sock ) )
{
}
