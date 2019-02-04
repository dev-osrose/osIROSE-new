#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliAlive : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_ALIVE;
        CliAlive();
        CliAlive(CRoseReader reader);
        CliAlive(CliAlive&&) = default;
        CliAlive& operator=(CliAlive&&) = default;
        ~CliAlive() = default;
        
        static constexpr size_t size();
        
        
        
        
        
        static CliAlive create();
        static CliAlive create(const uint8_t*);
        static std::unique_ptr<CliAlive> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
};

}
}
