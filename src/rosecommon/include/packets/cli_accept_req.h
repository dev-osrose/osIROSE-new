#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliAcceptReq : public CRosePacket {
    public:
        CliAcceptReq();
        CliAcceptReq(CRoseReader reader);
        CliAcceptReq(CliAcceptReq&&) = default;
        CliAcceptReq& operator=(CliAcceptReq&&) = default;
        ~CliAcceptReq() = default;
        
        static constexpr size_t size();
        
        
        
        
        
        static CliAcceptReq create();
        static CliAcceptReq create(const uint8_t*);
        static std::unique_ptr<CliAcceptReq> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
};

}
}
