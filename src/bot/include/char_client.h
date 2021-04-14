#pragma once

#include "client.h"

#include "packet_dispatcher.h"

#include "srv_accept_reply.h"
#include "srv_join_server_reply.h"
#include "srv_char_list_reply.h"
#include "srv_create_char_reply.h"
#include "srv_switch_server.h"

class CharClient : public Client {
    public:
    CharClient(uint32_t sessionId, std::unique_ptr<Core::INetwork> sock);
    virtual ~CharClient() = default;

    uint32_t sessionId() const;
    uint16_t port() const;
    uint32_t sessionSeed() const;
    const std::string &ip() const;

    private:
        uint32_t sessionId_;
        uint16_t port_;
        uint32_t sessionSeed_;
        std::string ip_;
        RoseCommon::PacketDispatcher<> dispatcher;

        virtual bool handlePacket(uint8_t *buffer) override;

        template <typename T>
        void register_dispatcher(std::function<void(const T&)>&& func) {
            dispatcher.add_dispatcher(T::PACKET_ID, std::move(func));
        }

        void accept_reply(const RoseCommon::Packet::SrvAcceptReply&);
        void join_server_reply(const RoseCommon::Packet::SrvJoinServerReply& reply);
        void char_list_reply(const RoseCommon::Packet::SrvCharListReply& reply);
        void create_char_reply(const RoseCommon::Packet::SrvCreateCharReply&);
        void switch_server(const RoseCommon::Packet::SrvSwitchServer& reply);
};