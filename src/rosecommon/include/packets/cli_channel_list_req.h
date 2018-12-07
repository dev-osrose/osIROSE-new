#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliChannelListReq : public CRosePacket {
    public:
        CliChannelListReq();
        CliChannelListReq(CRoseReader reader);
        CliChannelListReq(CliChannelListReq&&) = default;
        CliChannelListReq& operator=(CliChannelListReq&&) = default;
        ~CliChannelListReq() = default;
        
        static constexpr size_t size();
        
        
        
        void set_serverId(const uint32_t);
        uint32_t get_serverId() const;
        
        
        static CliChannelListReq create(const uint32_t&);
        static CliChannelListReq create(const uint8_t*);
        static std::unique_ptr<CliChannelListReq> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint32_t serverId;
};

}
}
