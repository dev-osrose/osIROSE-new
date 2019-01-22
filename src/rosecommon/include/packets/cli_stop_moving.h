#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliStopMoving : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_STOP_MOVING;
        CliStopMoving();
        CliStopMoving(CRoseReader reader);
        CliStopMoving(CliStopMoving&&) = default;
        CliStopMoving& operator=(CliStopMoving&&) = default;
        ~CliStopMoving() = default;
        
        static constexpr size_t size();
        
        
        
        void set_x(const float);
        float get_x() const;
        void set_y(const float);
        float get_y() const;
        void set_z(const uint16_t);
        uint16_t get_z() const;
        
        
        static CliStopMoving create(const float&, const float&, const uint16_t&);
        static CliStopMoving create(const uint8_t*);
        static std::unique_ptr<CliStopMoving> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        float x;
        float y;
        uint16_t z;
};

}
}
