#ifndef __MOCK_CROSECLIENT_H__
#define __MOCK_CROSECLIENT_H__

#include "gmock/gmock.h" // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "croseclient.h"
class CRoseClient_Mock : public CRoseClient
{
        public:
        protected:

	virtual bool OnSend( uint8_t* _buffer )
	{
		crypt_.encodeClientPacket( _buffer );
		return true;
	}

        private:
        CNetwork_Asio real_;
};

#endif
