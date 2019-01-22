#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliSrvSelectReq : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_SRV_SELECT_REQ;
        CliSrvSelectReq();
        CliSrvSelectReq(CRoseReader reader);
        CliSrvSelectReq(CliSrvSelectReq&&) = default;
        CliSrvSelectReq& operator=(CliSrvSelectReq&&) = default;
        ~CliSrvSelectReq() = default;
        
        static constexpr size_t size();
        
        
        
        void set_serverId(const uint32_t);
        uint32_t get_serverId() const;
        void set_channelId(const uint8_t);
        uint8_t get_channelId() const;
        
        
        static CliSrvSelectReq create(const uint32_t&, const uint8_t&);
        static CliSrvSelectReq create(const uint8_t*);
        static std::unique_ptr<CliSrvSelectReq> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint32_t serverId;
        uint8_t channelId;
};

}
}
