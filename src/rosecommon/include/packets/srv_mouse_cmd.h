#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class SrvMouseCmd : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_MOUSE_CMD;
        SrvMouseCmd();
        SrvMouseCmd(CRoseReader reader);
        SrvMouseCmd(SrvMouseCmd&&) = default;
        SrvMouseCmd& operator=(SrvMouseCmd&&) = default;
        ~SrvMouseCmd() = default;
        
        static constexpr size_t size();
        
        
        
        void set_id(const uint16_t);
        uint16_t get_id() const;
        void set_targetId(const uint16_t);
        uint16_t get_targetId() const;
        void set_distance(const uint16_t);
        uint16_t get_distance() const;
        void set_x(const float);
        float get_x() const;
        void set_y(const float);
        float get_y() const;
        void set_z(const uint16_t);
        uint16_t get_z() const;
        
        
        static SrvMouseCmd create(const uint16_t&);
        static SrvMouseCmd create(const uint8_t*);
        static std::unique_ptr<SrvMouseCmd> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t id;
        uint16_t targetId = 0;
        uint16_t distance = 0;
        float x = 0;
        float y = 0;
        uint16_t z = 0;
};

}
}
