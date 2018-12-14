#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class IscAlive : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::ISC_ALIVE;
        IscAlive();
        IscAlive(CRoseReader reader);
        IscAlive(IscAlive&&) = default;
        IscAlive& operator=(IscAlive&&) = default;
        ~IscAlive() = default;
        
        static constexpr size_t size();
        
        
        
        
        
        static IscAlive create();
        static IscAlive create(const uint8_t*);
        static std::unique_ptr<IscAlive> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
};

}
}
