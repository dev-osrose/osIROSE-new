#ifndef _CLOGINISC_H_
#define _CLOGINISC_H_

#include "croseisc.h"

class CLoginISC : public CRoseISC
{
public:
        CLoginISC( );
        CLoginISC( tcp::socket _sock );

	std::string GetName() { return name; }
	std::string GetIP() { return address; }
	uint16_t GetPort() { return port; }
	uint8_t GetServerType() { return type; }

protected:
	bool HandlePacket( uint8_t* _buffer );
	bool ServerRegister( CPacket* P );
	bool ServerShutdown( CPacket* P );

	std::string name;
	std::string address;
	uint16_t port;
	uint8_t type;
	//next type needs to be 8 bits for 4 byte alignment
};

#endif

