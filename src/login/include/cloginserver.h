#ifndef _CLOGINSERVER_H_
#define _CLOGINSERVER_H_

#include "croseserver.h"

class CLoginServer : public CRoseServer
{
public:
	CLoginServer( bool _isc = false );
	virtual ~CLoginServer();

protected:
	virtual void OnAccepted( tcp::socket _sock );
//	virtual bool HandlePacket( uint8_t* _buffer );
};

#endif
