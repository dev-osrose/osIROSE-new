#ifndef __MOCK_CLOGINCLIENT_H__
#define __MOCK_CLOGINCLIENT_H__

#include "gmock/gmock.h"  // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "cloginclient.h"
class CLoginClient_Mock : public CLoginClient {
 public:
 protected:
  virtual bool OnSend(uint8_t* _buffer) {
    crypt_.encodeClientPacket(_buffer);
    return true;
  }
};

#endif
