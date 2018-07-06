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

   virtual void set_socket(std::unique_ptr<Core::INetwork> _val) {
    CLoginClient::set_socket(std::move(_val));

    socket_[0]->registerOnSend(std::bind(&CLoginClient_Mock::OnSend, this, std::placeholders::_1));
   }

 protected:

  virtual bool OnSend(uint8_t* _buffer) {
    crypt_.encodeClientPacket(_buffer);
    return true;
  }
};

#endif
