#ifndef MOCK_CROSECLIENT_H_
#define MOCK_CROSECLIENT_H_

#include "gmock/gmock.h"  // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "croseclient.h"

namespace RoseCommon {
 
class MockCRoseClient : public CRoseClient {
 public:
 
 MockCRoseClient() {
   // By default, all calls are delegated to the real object.
    ON_CALL(*this, Send())
        .WillByDefault(Invoke(&real_, &CRoseClient::Send));
    ON_CALL(*this, OnConnect())
        .WillByDefault(Invoke(&real_, &CRoseClient::OnConnect));
 }
 
  MOCK_METHOD1(Send,
      bool(CRosePacket &_buffer));
  MOCK_METHOD1(Send,
      bool(std::unique_ptr<uint8_t[]> _buffer));
  MOCK_METHOD0(OnConnect,
      bool());
  MOCK_METHOD0(OnConnected,
      void());
  MOCK_METHOD0(OnDisconnect,
      bool());
  MOCK_METHOD0(OnDisconnected,
      void());
  MOCK_METHOD0(OnReceive,
      bool());
  MOCK_METHOD0(OnReceived,
      bool());
  MOCK_METHOD1(OnSend,
      bool(uint8_t* _buffer));
  MOCK_METHOD0(OnSent,
      void());
  MOCK_METHOD1(HandlePacket,
      bool(uint8_t* _buffer));
      
//  virtual bool OnSend(uint8_t* _buffer) {
//    crypt_.encodeClientPacket(_buffer);
//    return true;
//  }

private:
  CRoseClient real_;
};

}  // namespace RoseCommon

#endif
