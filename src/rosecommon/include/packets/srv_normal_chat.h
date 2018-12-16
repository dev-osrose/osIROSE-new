#pragma once


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class SrvNormalChat : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_NORMAL_CHAT;
        SrvNormalChat();
        SrvNormalChat(CRoseReader reader);
        SrvNormalChat(SrvNormalChat&&) = default;
        SrvNormalChat& operator=(SrvNormalChat&&) = default;
        ~SrvNormalChat() = default;
        
        static constexpr size_t size();
        
        
        
        void set_charId(const uint16_t);
        uint16_t get_charId() const;
        void set_message(const std::string&);
        const std::string& get_message() const;
        
        
        static SrvNormalChat create(const uint16_t&, const std::string&);
        static SrvNormalChat create(const uint8_t*);
        static std::unique_ptr<SrvNormalChat> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t charId;
        std::string message;
};

}
}
