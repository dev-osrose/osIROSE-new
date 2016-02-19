#ifndef _CLOGINISC_H_
#define _CLOGINISC_H_

#include <forward_list>
#include "croseisc.h"

struct channelInfo;

class CLoginISC : public CRoseISC
{
public:
        CLoginISC( );
        CLoginISC( tcp::socket _sock );

	std::string GetName() { return name; }
	std::string GetIP() { return m_IpAddress; }
	uint16_t GetPort() { return m_wPort; }
	uint8_t GetServerType() { return m_iType; }
	bool IsTestServer() { return testServer; }

protected:
	bool HandlePacket( uint8_t* _buffer );
	bool ServerRegister( CPacket* P );
	bool ServerShutdown( CPacket* P );

	std::string name;
	bool testServer;

	std::forward_list< channelInfo > m_ChannelList;
};

#endif

