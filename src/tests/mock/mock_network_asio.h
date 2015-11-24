#include "gmock/gmock.h"  // Brings in Google Mock.

#include "cnetwork_asio.h"
class CNetwork_Asio_Mock : public CNetwork_Asio
{
public:
	MOCK_METHOD0( OnConnect, bool());
	MOCK_METHOD0( OnConnected, bool() );
	MOCK_METHOD0( OnListen, bool() );
	MOCK_METHOD0( OnListening, bool() );
	MOCK_METHOD0( OnDisconnect, bool() );
	MOCK_METHOD0( OnDisconnected, bool() );
	MOCK_METHOD0( OnReceive, bool() );
	MOCK_METHOD2( OnReceived, bool(uint8_t* _buffer, uint16_t _size) );
	MOCK_METHOD2( OnSend, bool(uint8_t* _buffer, uint16_t _size) );
	MOCK_METHOD0( OnSent, bool() );
};
