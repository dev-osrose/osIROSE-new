#ifndef __CCHARISC_H__
#define __CCHARISC_H__

#include "croseisc.h"

class CCharISC : public CRoseISC
{
public:
	CCharISC( );
	CCharISC( tcp::socket _sock );

	bool IsLogin() const
	{
		return m_LoginConnection;
	}
	void SetLogin( bool val )
	{
		m_LoginConnection = val;
	}

protected:
	bool         ServerRegister( CPacket* P );
	bool         HandlePacket( uint8_t* _buffer );
	virtual void OnConnected( );

	bool m_LoginConnection;
};

#endif
