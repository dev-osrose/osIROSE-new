#ifndef __CCHARISC_H__
#define __CCHARISC_H__

#include "croseisc.h"
#include "crosepacket.h"

class CCharISC : public CRoseISC
{
public:
	CCharISC( );
	CCharISC( tcp::socket _sock );

	bool IsLogin() const
	{
		return login_connection_;
	}
	void SetLogin( bool val )
	{
		login_connection_ = val;
	}

protected:
	bool         ServerRegister( CRosePacket* P );
	bool         HandlePacket( uint8_t* _buffer );
	virtual void OnConnected( );

	bool login_connection_;
};

#endif
