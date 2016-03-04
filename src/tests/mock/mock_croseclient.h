#ifndef __MOCK_CROSECLIENT_H__
#define __MOCK_CROSECLIENT_H__

#include "gmock/gmock.h"  // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "croseclient.h"
class CRoseClient_Mock : public CRoseClient {
 public:
 protected:
  virtual bool OnSend(uint8_t* _buffer) {
    crypt_.encodeClientPacket(_buffer);
    return true;
  }

  virtual bool HandlePacket(uint8_t* _buffer) {
    CRosePacket* pak = (CRosePacket*)_buffer;
    switch ((ePacketType)pak->Header.Command) {
      case ePacketType::PAKCS_ALIVE: {
        log_.icprintf("[%d] Got keep alive packet\n");
        break;
      }
      default: {
        log_.eicprintf("Unknown Packet Type: 0x%X\n", pak->Header.Command);
        return false;
      }
    }
    return true;
  }

 private:
  CRoseClient real_;
};

#endif
