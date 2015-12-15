#ifndef _CLOGINCLIENT_H_
#define _CLOGINCLIENT_H_

#include "croseclient.h"

struct CPacket;

class CLoginClient : public CRoseClient
{
public:
	CLoginClient( );
	CLoginClient( tcp::socket _sock );

	bool UserLogin( CPacket* P );
	bool ChannelList( CPacket* P );
	bool ServerSelect( CPacket* P );

protected:
	virtual bool HandlePacket( uint8_t* _buffer );
	virtual bool OnReceived( );


	// Packet Helper Functions
	void SendLoginReply( uint8_t Result );

	uint16_t m_Right;
};

#endif
