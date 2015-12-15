#ifndef __MOCK_CLOGINISC_H__
#define __MOCK_CLOGINISC_H__

#include "gmock/gmock.h" // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "cloginisc.h"
class CLoginISC_Mock : public CLoginISC
{
        public:
        protected:
        virtual bool OnSend( uint8_t* _buffer )
        {
                m_Crypt.encodeClientPacket( _buffer );
                return true;
        }
};

#endif
