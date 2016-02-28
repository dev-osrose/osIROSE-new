#ifndef _CLOGINCLIENT_H_
#define _CLOGINCLIENT_H_

#include "croseclient.h"
#include "crosepacket.h"

class CLoginClient : public CRoseClient
{
	public:
	CLoginClient( );
	CLoginClient( tcp::socket _sock );

	protected:
	virtual bool HandlePacket( uint8_t* _buffer );
	virtual bool OnReceived( );

	// Packet Helper Functions
	bool UserLogin( CRosePacket* P );
	bool ChannelList( CRosePacket* P );
	bool ServerSelect( CRosePacket* P );

	void SendLoginReply( uint8_t Result );

	uint16_t access_rights_;
	uint8_t  username_[ 17 ];
	uint8_t  password_[ 33 ];
};

#endif
