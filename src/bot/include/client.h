#pragma once

#include "crosesocket.h"
#include "inetwork.h"

class Client : public RoseCommon::CRoseSocket {
    public:
    Client(std::unique_ptr<Core::INetwork> sock);
    virtual ~Client() = default;

    protected:
        virtual void onConnected();
        virtual bool onSend(uint16_t socketId, uint8_t *buffer);
        virtual bool onReceived(uint16_t socketId, uint16_t& packetSize, uint8_t *buffer);
};