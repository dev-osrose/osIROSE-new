#ifndef __MOCK_CLOGINISC_H__
#define __MOCK_CLOGINISC_H__

#include "gmock/gmock.h"  // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "cloginisc.h"
class CLoginISC_Mock : public CLoginISC {
 public:
  CLoginISC_Mock() : CLoginISC() { }

  CLoginISC_Mock(tcp::socket _sock) : CLoginISC(std::move(_sock)) {
  }

 protected:
  virtual bool OnSend(uint8_t* _buffer) {
    (void)_buffer;
    //                m_Crypt.encodeClientPacket( _buffer );
    return true;
  }
};

#endif
