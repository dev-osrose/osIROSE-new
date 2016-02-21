#ifndef _CLOGINISC_H_
#define _CLOGINISC_H_

#include <forward_list>
#include "croseisc.h"

struct tChannelInfo;

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
	uint32_t m_ChannelCount;
	uint32_t m_MinRight;
	bool testServer;
	
	std::forward_list< tChannelInfo > m_ChannelList;
};

#endif

