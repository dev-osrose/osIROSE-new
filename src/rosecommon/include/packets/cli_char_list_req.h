#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliCharListReq : public CRosePacket {
    public:
        CliCharListReq();
        CliCharListReq(CRoseReader reader);
        CliCharListReq(CliCharListReq&&) = default;
        CliCharListReq& operator=(CliCharListReq&&) = default;
        ~CliCharListReq() = default;
        
        static constexpr size_t size();
        
        
        
        
        
        static CliCharListReq create();
        static CliCharListReq create(const uint8_t*);
        static std::unique_ptr<CliCharListReq> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
};

}
}
