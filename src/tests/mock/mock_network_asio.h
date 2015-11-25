#include "gmock/gmock.h"  // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "cnetwork_asio.h"
class CNetwork_Asio_Mock : public CNetwork_Asio
{
public:
	/*CNetwork_Asio_Mock()
	{
		ON_CALL(*this, OnConnected())
			.WillByDefault(Invoke(&real_, CNetwork_Asio::OnConnected()));
		ON_CALL(*this, OnListen())
					.WillByDefault(Invoke(&real_, &CNetwork_Asio::OnListen));
		ON_CALL(*this, OnListening())
					.WillByDefault(Invoke(&real_, &CNetwork_Asio::OnListening));
		ON_CALL(*this, OnDisconnect())
					.WillByDefault(Invoke(&real_, &CNetwork_Asio::OnDisconnect));
		ON_CALL(*this, OnDisconnected())
					.WillByDefault(Invoke(&real_, &CNetwork_Asio::OnDisconnected));
		ON_CALL(*this, OnReceive())
					.WillByDefault(Invoke(&real_, &CNetwork_Asio::OnReceive));
		ON_CALL(*this, OnReceived(_,_))
					.WillByDefault(Invoke(&real_, &CNetwork_Asio::OnReceived));
		ON_CALL(*this, OnSend(_,_))
					.WillByDefault(Invoke(&real_, &CNetwork_Asio::OnSend));
		ON_CALL(*this, OnSent())
					.WillByDefault(Invoke(&real_, &CNetwork_Asio::OnSent));
	}*/
//protected:
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

private:
	CNetwork_Asio real_;
};
