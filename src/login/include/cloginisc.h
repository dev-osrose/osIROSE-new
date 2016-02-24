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

	std::string GetName( ) { return server_name_; }
	std::string GetIP( ) { return m_IpAddress; }
	uint16_t    GetPort( ) { return m_wPort; }
	uint8_t     GetServerType( ) { return m_iType; }
	bool        IsTestServer( ) { return test_server_; }

protected:
	bool HandlePacket( uint8_t* _buffer );
	bool ServerRegister( CPacket* P );
	bool ServerShutdown( CPacket* P );

	std::string server_name_;
	uint32_t    channel_count_;
	uint32_t    min_right_;
	bool        test_server_;

	std::forward_list< tChannelInfo > channel_list_;
};

#endif
