#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliPartyReply : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_PARTY_REPLY;
        CliPartyReply();
        CliPartyReply(CRoseReader reader);
        CliPartyReply(CliPartyReply&&) = default;
        CliPartyReply& operator=(CliPartyReply&&) = default;
        ~CliPartyReply() = default;
        
        static constexpr size_t size();
        
        
        enum Reply : uint8_t {
            NOT_FOUND = 0,
            BUSY = 1,
            ACCEPT_MAKE = 2,
            ACCEPT_JOIN = 3,
            REJECT = 4,
            DESTROY = 5,
            FULL_MEMBERS = 6,
            INVALID_LEVEL = 7,
            CHANGE_OWNER = 8,
            CHANGE_OWNER_DISCONNECT = 9,
            NO_CHANGE_TARGET = 10,
            KICK = 128,
            DISCONNECT = 129,
            REJOIN = 130,
        };
        
        
        void set_reply(const Reply);
        Reply get_reply() const;
        void set_idXorTag(const uint32_t);
        uint32_t get_idXorTag() const;
        
        
        static CliPartyReply create(const Reply&, const uint32_t&);
        static CliPartyReply create(const uint8_t*);
        static std::unique_ptr<CliPartyReply> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        Reply reply;
        uint32_t idXorTag;
};

}
}
