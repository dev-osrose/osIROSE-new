#ifndef MOCK_CLOGINCLIENT_H_
#define MOCK_CLOGINCLIENT_H_

#include "gmock/gmock.h"  // Brings in Google Mock.

using ::testing::_;
using ::testing::Invoke;

#include "cloginclient.h"
class CLoginClient_Mock : public CLoginClient {
 public:
   CLoginClient_Mock() : CLoginClient() {
   }

   virtual void set_socket(Core::INetwork* _val) {
    CLoginClient::set_socket(_val);

    std::function<bool(uint8_t*)> fnOnSend = std::bind(&CLoginClient_Mock::OnSend, this, std::placeholders::_1);
    socket_->registerOnSend(fnOnSend);
   }

 protected:
  virtual bool OnSend(uint8_t* _buffer) {
    crypt_.encodeClientPacket(_buffer);
    return true;
  }
};

#endif
