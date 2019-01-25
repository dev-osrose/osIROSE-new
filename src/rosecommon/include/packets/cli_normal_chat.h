#pragma once


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class CliNormalChat : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_NORMAL_CHAT;
        CliNormalChat();
        CliNormalChat(CRoseReader reader);
        CliNormalChat(CliNormalChat&&) = default;
        CliNormalChat& operator=(CliNormalChat&&) = default;
        ~CliNormalChat() = default;
        
        static constexpr size_t size();
        
        
        
        void set_message(const std::string&);
        const std::string& get_message() const;
        
        
        static CliNormalChat create(const std::string&);
        static CliNormalChat create(const uint8_t*);
        static std::unique_ptr<CliNormalChat> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        std::string message;
};

}
}
