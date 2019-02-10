#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class SrvPartyReq : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_PARTY_REQ;
        SrvPartyReq();
        SrvPartyReq(CRoseReader reader);
        SrvPartyReq(SrvPartyReq&&) = default;
        SrvPartyReq& operator=(SrvPartyReq&&) = default;
        ~SrvPartyReq() = default;
        
        static constexpr size_t size();
        
        
        enum Request : uint8_t {
            MAKE = 0,
            JOIN = 1,
            LEFT = 2,
            CHANGE_OWNER = 3,
            KICK = 128,
        };
        
        
        void set_request(const Request);
        Request get_request() const;
        void set_tag(const uint32_t);
        uint32_t get_tag() const;
        void set_name(const std::string&);
        const std::string& get_name() const;
        
        
        static SrvPartyReq create(const Request&, const uint32_t&, const std::string&);
        static SrvPartyReq create(const uint8_t*);
        static std::unique_ptr<SrvPartyReq> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        Request request;
        uint32_t tag;
        std::string name;
};

}
}
