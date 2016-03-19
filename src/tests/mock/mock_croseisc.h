#ifndef __MOCK_CROSEISC_H__
#define __MOCK_CROSEISC_H__

#include "gmock/gmock.h"  // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "croseisc.h"
class CRoseISC_Mock : public RoseCommon::CRoseISC {
 public:
  CRoseISC_Mock() : CRoseISC() { }

  CRoseISC_Mock(tcp::socket _sock) : CRoseISC(std::move(_sock)) {
  }

 protected:
  virtual bool OnSend(uint8_t* _buffer) {
    (void)_buffer;
    //                m_Crypt.encodeClientPacket( _buffer );
    return true;
  }
};

#endif
