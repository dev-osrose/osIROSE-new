#include "cloginclient.h"

CLoginClient::CLoginClient() : CRoseClient()
{
}

CLoginClient::CLoginClient( tcp::socket _sock ) : CRoseClient( std::move( _sock ) )
{
}
