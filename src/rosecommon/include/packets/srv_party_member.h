#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"
#include "dataconsts.h"

namespace RoseCommon {
namespace Packet {

class SrvPartyMember : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_PARTY_MEMBER;
        SrvPartyMember();
        SrvPartyMember(CRoseReader reader);
        SrvPartyMember(SrvPartyMember&&) = default;
        SrvPartyMember& operator=(SrvPartyMember&&) = default;
        ~SrvPartyMember() = default;
        
        static constexpr size_t size();
        
        
        
        void set_rules(const uint8_t);
        uint8_t get_rules() const;
        void set_data(const PartyData);
        PartyData get_data() const;
        
        
        static SrvPartyMember create(const uint8_t&, const PartyData&);
        static SrvPartyMember create(const uint8_t*);
        static std::unique_ptr<SrvPartyMember> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint8_t rules;
        PartyData data;
};

}
}
