#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliPartyReq : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_PARTY_REQ;
        CliPartyReq();
        CliPartyReq(CRoseReader reader);
        CliPartyReq(CliPartyReq&&) = default;
        CliPartyReq& operator=(CliPartyReq&&) = default;
        ~CliPartyReq() = default;
        
        static constexpr size_t size();
        
        
        enum Request : uint8_t {
            MAKE = 0,
            JOIN = 1,
            LEFT = 2,
            CHANGE_OWNER = 3,
            KICK = 129,
        };
        
        
        void set_request(const Request);
        Request get_request() const;
        void set_idXorTag(const uint32_t);
        uint32_t get_idXorTag() const;
        
        
        static CliPartyReq create(const Request&, const uint32_t&);
        static CliPartyReq create(const uint8_t*);
        static std::unique_ptr<CliPartyReq> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        Request request;
        uint32_t idXorTag;
};

}
}
