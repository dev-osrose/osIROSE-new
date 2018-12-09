#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliAlive : public CRosePacket {
    public:
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
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
};

}
}
