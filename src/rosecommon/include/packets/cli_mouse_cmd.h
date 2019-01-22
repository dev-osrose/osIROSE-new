#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class CliMouseCmd : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_MOUSE_CMD;
        CliMouseCmd();
        CliMouseCmd(CRoseReader reader);
        CliMouseCmd(CliMouseCmd&&) = default;
        CliMouseCmd& operator=(CliMouseCmd&&) = default;
        ~CliMouseCmd() = default;
        
        static constexpr size_t size();
        
        
        
        void set_targetId(const uint16_t);
        uint16_t get_targetId() const;
        void set_x(const float);
        float get_x() const;
        void set_y(const float);
        float get_y() const;
        void set_z(const uint16_t);
        uint16_t get_z() const;
        
        
        static CliMouseCmd create(const uint16_t&, const float&, const float&, const uint16_t&);
        static CliMouseCmd create(const uint8_t*);
        static std::unique_ptr<CliMouseCmd> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t targetId;
        float x;
        float y;
        uint16_t z;
};

}
}
