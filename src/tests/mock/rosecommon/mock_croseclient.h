#ifndef __MOCK_CROSECLIENT_H__
#define __MOCK_CROSECLIENT_H__

#include "gmock/gmock.h"  // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "croseclient.h"
class CRoseClient_Mock : public RoseCommon::CRoseClient {
 public:
 protected:
  virtual bool OnSend(uint8_t* _buffer) {
    crypt_.encodeClientPacket(_buffer);
    return true;
  }

  virtual bool HandlePacket(uint8_t* _buffer) {
	  RoseCommon::CRosePacket* pak = (RoseCommon::CRosePacket*)_buffer;
    switch ((RoseCommon::ePacketType)pak->Header.Command) {
		case RoseCommon::ePacketType::PAKCS_ALIVE: {
        //logger_->notice("Got keep alive packet\n");
        break;
      }
      default: {
        //logger_->error("Unknown Packet Type: 0x%X\n", pak->Header.Command);
        return false;
      }
    }
    return true;
  }

 private:
  RoseCommon::CRoseClient real_;
};

#endif
