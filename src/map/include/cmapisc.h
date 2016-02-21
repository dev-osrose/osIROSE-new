#ifndef __CMAPISC_H__
#define __CMAPISC_H__

#include "croseisc.h"

class CMapISC : public CRoseISC
{
	public:
	CMapISC( );
	CMapISC( tcp::socket _sock );

	bool IsChar( ) const
	{
		return m_CharServer;
	}
	void SetChar( bool val )
	{
		m_CharServer = val;
	}

	protected:
	bool         HandlePacket( uint8_t* _buffer );
	virtual void OnConnected( );
	bool         m_CharServer;
};

#endif
