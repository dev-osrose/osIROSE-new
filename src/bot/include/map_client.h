#pragma once

#include "client.h"

#include "packet_dispatcher.h"

#include "srv_accept_reply.h"
#include "srv_join_server_reply.h"
#include "srv_select_char_reply.h"
#include "srv_inventory_data.h"
#include "srv_quest_data.h"
#include "srv_billing_message.h"
#include "srv_change_map_reply.h"
#include "srv_whisper_chat.h"

class MapClient : public Client {
    public:
        MapClient(uint32_t sessionId, uint32_t sessionSeed, std::unique_ptr<Core::INetwork> sock);
        virtual ~MapClient() = default;

    private:
        uint32_t sessionId_;
        uint32_t sessionSeed_;
        RoseCommon::PacketDispatcher<> dispatcher;

        template <typename T>
        void register_dispatcher(std::function<void(const T&)>&& func) {
            dispatcher.add_dispatcher(T::PACKET_ID, std::move(func));
        }

        virtual bool handlePacket(uint8_t *buffer) override;

        void accept_reply(const RoseCommon::Packet::SrvAcceptReply&);
        void join_server_reply(const RoseCommon::Packet::SrvJoinServerReply&);
        void select_char_reply(const RoseCommon::Packet::SrvSelectCharReply&);
        void inventory_data(const RoseCommon::Packet::SrvInventoryData&);
        void quest_data(const RoseCommon::Packet::SrvQuestData&);
        void billing_message(const RoseCommon::Packet::SrvBillingMessage&);
        void change_map_reply(const RoseCommon::Packet::SrvChangeMapReply&);
        void whisper_chat(const RoseCommon::Packet::SrvWhisperChat& packet);
};