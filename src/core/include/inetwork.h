#ifndef INETWORK_H_
#define INETWORK_H_

//TODO: Add parameters for the functions
#include <stdint.h>

class INetwork
{
	public:
	INetwork( )
	    : m_iId( 0 ), m_iType( 0 ), m_wPort( 0 ), m_IpAddress( "" )
	{
	}
	virtual ~INetwork( ) {}

	virtual bool Init( std::string _ip, uint16_t _port ) = 0;
	virtual bool Shutdown( ) = 0;

	virtual bool Connect( ) = 0;
	virtual bool Listen( ) = 0;
	virtual bool Reconnect( ) = 0;
	virtual bool Disconnect( ) = 0;

	virtual uint32_t    GetId( ) { return m_iId; }
	virtual uint32_t    GetType( ) { return m_iType; }
	virtual uint16_t    GetPort( ) { return m_wPort; }
	virtual std::string GetIpAddress( ) { return m_IpAddress; }

	protected:
	virtual bool Send( uint8_t* _buffer ) = 0;
	virtual bool Recv( uint16_t _size = 6 ) = 0;

	// Callback functions
	virtual bool OnConnect( ) = 0;
	virtual void OnConnected( ) = 0;
	virtual bool OnListen( ) = 0;
	virtual void OnListening( ) = 0;
	virtual bool OnDisconnect( ) = 0;
	virtual void OnDisconnected( ) = 0;
	virtual bool OnReceive( ) = 0;
	virtual bool OnReceived( uint8_t* _buffer, uint16_t _size ) = 0;
	virtual bool OnSend( uint8_t* _buffer ) = 0;
	virtual void OnSent( ) = 0;

	//private:
	uint32_t m_iId;
	uint32_t m_iType;
	uint16_t m_wPort;

	std::string m_IpAddress;
};

#endif
