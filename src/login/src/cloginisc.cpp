#include "cloginisc.h"
#include "ePacketType.h"

CLoginISC::CLoginISC() : CRoseISC()
{
	m_Log.SetIdentity( "CLoginISC" );
}

CLoginISC::CLoginISC( tcp::socket _sock ) : CRoseISC( std::move( _sock ) )
{
	m_Log.SetIdentity( "CLoginISC" );
}

bool CLoginISC::HandlePacket( uint8_t* _buffer )
{
	CPacket* pak = (CPacket*)_buffer;
	switch ( pak->Header.Command )
	{
	default:
	{
		CRoseISC::HandlePacket( _buffer );
		return false;
	}
	}
	return true;
}
