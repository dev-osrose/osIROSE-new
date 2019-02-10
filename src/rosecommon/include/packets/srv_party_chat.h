#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class SrvPartyChat : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_PARTY_CHAT;
        SrvPartyChat();
        SrvPartyChat(CRoseReader reader);
        SrvPartyChat(SrvPartyChat&&) = default;
        SrvPartyChat& operator=(SrvPartyChat&&) = default;
        ~SrvPartyChat() = default;
        
        static constexpr size_t size();
        
        
        
        void set_charId(const uint16_t);
        uint16_t get_charId() const;
        void set_message(const std::string&);
        const std::string& get_message() const;
        
        
        static SrvPartyChat create(const uint16_t&, const std::string&);
        static SrvPartyChat create(const uint8_t*);
        static std::unique_ptr<SrvPartyChat> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t charId;
        std::string message;
};

}
}
