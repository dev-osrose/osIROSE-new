#ifndef MOCK_CLOGINISC_H_
#define MOCK_CLOGINISC_H_

#include "gmock/gmock.h"  // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "cloginisc.h"
class CLoginISC_Mock : public CLoginISC {
 public:
  CLoginISC_Mock() : CLoginISC() { }

  CLoginISC_Mock(Core::INetwork* _sock) : CLoginISC(std::move(_sock)) {
  }

 protected:
  virtual bool OnSend(uint8_t* _buffer) {
    (void)_buffer;
    //                m_Crypt.encodeClientPacket( _buffer );
    return true;
  }
};

#endif
