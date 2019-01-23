#pragma once


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class SrvSwitchServer : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCC_SWITCH_SERVER;
        SrvSwitchServer();
        SrvSwitchServer(CRoseReader reader);
        SrvSwitchServer(SrvSwitchServer&&) = default;
        SrvSwitchServer& operator=(SrvSwitchServer&&) = default;
        ~SrvSwitchServer() = default;
        
        static constexpr size_t size();
        
        
        
        void set_port(const uint16_t);
        uint16_t get_port() const;
        void set_sessionId(const uint32_t);
        uint32_t get_sessionId() const;
        void set_sessionSeed(const uint32_t);
        uint32_t get_sessionSeed() const;
        void set_ip(const std::string&);
        const std::string& get_ip() const;
        
        
        static SrvSwitchServer create(const uint16_t&, const uint32_t&, const uint32_t&, const std::string&);
        static SrvSwitchServer create(const uint8_t*);
        static std::unique_ptr<SrvSwitchServer> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t port;
        uint32_t sessionId;
        uint32_t sessionSeed;
        std::string ip;
};

}
}
