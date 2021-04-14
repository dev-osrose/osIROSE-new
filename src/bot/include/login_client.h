#pragma once

#include "client.h"

#include "packet_dispatcher.h"

#include "srv_accept_reply.h"
#include "srv_login_reply.h"
#include "srv_channel_list_reply.h"
#include "srv_srv_select_reply.h"

class LoginClient : public Client {
    public:
        LoginClient(std::unique_ptr<Core::INetwork> sock);
        virtual ~LoginClient() = default;

        uint32_t sessionId() const;

        std::string ip() const;

        uint16_t port() const;

    private:
        uint32_t sessionId_;
        std::string ip_;
        uint16_t port_;
        RoseCommon::PacketDispatcher<> dispatcher;

        virtual bool handlePacket(uint8_t *buffer) override;

        template <typename T>
        void register_dispatcher(std::function<void(const T&)>&& func) {
            dispatcher.add_dispatcher(T::PACKET_ID, std::move(func));
        }

        void accept_reply(const RoseCommon::Packet::SrvAcceptReply&);
        void login_reply(const RoseCommon::Packet::SrvLoginReply& reply);
        void channel_list_reply(const RoseCommon::Packet::SrvChannelListReply& reply);
        void srv_select_reply(const RoseCommon::Packet::SrvSrvSelectReply& reply);
};