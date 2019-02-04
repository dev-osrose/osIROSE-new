#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class CliWhisperChat : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_WHISPER_CHAT;
        CliWhisperChat();
        CliWhisperChat(CRoseReader reader);
        CliWhisperChat(CliWhisperChat&&) = default;
        CliWhisperChat& operator=(CliWhisperChat&&) = default;
        ~CliWhisperChat() = default;
        
        static constexpr size_t size();
        
        
        
        void set_target(const std::string&);
        const std::string& get_target() const;
        void set_message(const std::string&);
        const std::string& get_message() const;
        
        
        static CliWhisperChat create(const std::string&, const std::string&);
        static CliWhisperChat create(const uint8_t*);
        static std::unique_ptr<CliWhisperChat> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        std::string target;
        std::string message;
};

}
}
