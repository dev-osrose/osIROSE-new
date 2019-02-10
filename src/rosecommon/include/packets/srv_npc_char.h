#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class SrvNpcChar : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_NPC_CHAR;
        SrvNpcChar();
        SrvNpcChar(CRoseReader reader);
        SrvNpcChar(SrvNpcChar&&) = default;
        SrvNpcChar& operator=(SrvNpcChar&&) = default;
        ~SrvNpcChar() = default;
        
        static constexpr size_t size();
        
        
        
        void set_id(const uint16_t);
        uint16_t get_id() const;
        void set_x(const float);
        float get_x() const;
        void set_y(const float);
        float get_y() const;
        void set_destX(const float);
        float get_destX() const;
        void set_destY(const float);
        float get_destY() const;
        void set_command(const uint16_t);
        uint16_t get_command() const;
        void set_targetId(const uint16_t);
        uint16_t get_targetId() const;
        void set_moveMode(const uint8_t);
        uint8_t get_moveMode() const;
        void set_hp(const int32_t);
        int32_t get_hp() const;
        void set_teamId(const int32_t);
        int32_t get_teamId() const;
        void set_statusFlag(const uint32_t);
        uint32_t get_statusFlag() const;
        void set_npcId(const uint16_t);
        uint16_t get_npcId() const;
        void set_questId(const uint16_t);
        uint16_t get_questId() const;
        void set_angle(const float);
        float get_angle() const;
        void set_eventStatus(const uint16_t);
        uint16_t get_eventStatus() const;
        
        
        static SrvNpcChar create(const uint16_t&);
        static SrvNpcChar create(const uint8_t*);
        static std::unique_ptr<SrvNpcChar> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t id;
        float x = 0;
        float y = 0;
        float destX = 0;
        float destY = 0;
        uint16_t command = 0;
        uint16_t targetId = 0;
        uint8_t moveMode = 0;
        int32_t hp = 0;
        int32_t teamId = 0;
        uint32_t statusFlag = 0;
        uint16_t npcId = 0;
        uint16_t questId = 0;
        float angle = 0;
        uint16_t eventStatus = 0;
};

}
}
