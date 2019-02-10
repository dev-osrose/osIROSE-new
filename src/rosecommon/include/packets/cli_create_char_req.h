#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class CliCreateCharReq : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_CREATE_CHAR_REQ;
        CliCreateCharReq();
        CliCreateCharReq(CRoseReader reader);
        CliCreateCharReq(CliCreateCharReq&&) = default;
        CliCreateCharReq& operator=(CliCreateCharReq&&) = default;
        ~CliCreateCharReq() = default;
        
        static constexpr size_t size();
        
        
        
        void set_race(const uint8_t);
        uint8_t get_race() const;
        void set_stone(const uint8_t);
        uint8_t get_stone() const;
        void set_hair(const uint8_t);
        uint8_t get_hair() const;
        void set_face(const uint8_t);
        uint8_t get_face() const;
        void set_weapon(const uint8_t);
        uint8_t get_weapon() const;
        void set_zone(const uint16_t);
        uint16_t get_zone() const;
        void set_name(const std::string&);
        const std::string& get_name() const;
        
        
        static CliCreateCharReq create(const uint8_t&, const uint8_t&, const uint8_t&, const uint8_t&, const uint8_t&, const uint16_t&, const std::string&);
        static CliCreateCharReq create(const uint8_t*);
        static std::unique_ptr<CliCreateCharReq> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint8_t race;
        uint8_t stone;
        uint8_t hair;
        uint8_t face;
        uint8_t weapon;
        uint16_t zone;
        std::string name;
};

}
}
