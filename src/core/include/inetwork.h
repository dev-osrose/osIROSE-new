#ifndef INETWORK_H_
#define INETWORK_H_

//TODO: Add parameters for the functions
#include <stdint.h>

class INetwork
{
	public:
	INetwork( )
	    : network_id_( 0 ), network_type_( 0 ), network_port_( 0 ), network_ip_address( "" )
	{
	}
	virtual ~INetwork( ) {}

	virtual bool Init( std::string _ip, uint16_t _port ) = 0;
	virtual bool Shutdown( ) = 0;

	virtual bool Connect( ) = 0;
	virtual bool Listen( ) = 0;
	virtual bool Reconnect( ) = 0;
	virtual bool Disconnect( ) = 0;

	virtual void    SetId( uint32_t _val ) { network_id_ = _val; }
        virtual void    SetType( uint32_t _val ) { network_type_ = _val; }

	virtual uint32_t    GetId( ) { return network_id_; }
	virtual uint32_t    GetType( ) { return network_type_; }
	virtual uint16_t    GetPort( ) { return network_port_; }
	virtual std::string GetIpAddress( ) { return network_ip_address; }

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
	virtual bool OnReceived( ) = 0;
	virtual bool OnSend( uint8_t* _buffer ) = 0;
	virtual void OnSent( ) = 0;

	//private:
	uint32_t network_id_;
	uint32_t network_type_;
	uint16_t network_port_;

	std::string network_ip_address;
};

#endif
