#ifndef _CLOGINSERVER_H_
#define _CLOGINSERVER_H_

#include "croseserver.h"

class CLoginServer : public CRoseServer
{
public:
	CLoginServer( bool _isc = false );
	virtual ~CLoginServer();

    uint32_t GetClientCount() { return (client_list_.empty() ? 0 : std::distance(std::begin(client_list_), std::end(client_list_))); }

protected:
	virtual void OnAccepted( tcp::socket _sock );
//	virtual bool HandlePacket( uint8_t* _buffer );

	uint32_t client_count_;
};

#endif
