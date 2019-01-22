#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliChangeMapReq : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_CHANGE_MAP_REQ;
        CliChangeMapReq();
        CliChangeMapReq(CRoseReader reader);
        CliChangeMapReq(CliChangeMapReq&&) = default;
        CliChangeMapReq& operator=(CliChangeMapReq&&) = default;
        ~CliChangeMapReq() = default;
        
        static constexpr size_t size();
        
        
        
        void set_weightRate(const uint8_t);
        uint8_t get_weightRate() const;
        void set_z(const uint16_t);
        uint16_t get_z() const;
        
        
        static CliChangeMapReq create(const uint8_t&, const uint16_t&);
        static CliChangeMapReq create(const uint8_t*);
        static std::unique_ptr<CliChangeMapReq> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint8_t weightRate;
        uint16_t z;
};

}
}
