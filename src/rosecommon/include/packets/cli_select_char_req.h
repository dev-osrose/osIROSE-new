#pragma once


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class CliSelectCharReq : public CRosePacket {
    public:
        CliSelectCharReq();
        CliSelectCharReq(CRoseReader reader);
        CliSelectCharReq(CliSelectCharReq&&) = default;
        CliSelectCharReq& operator=(CliSelectCharReq&&) = default;
        ~CliSelectCharReq() = default;
        
        static constexpr size_t size();
        
        
        
        void set_charId(const uint8_t);
        uint8_t get_charId() const;
        void set_runMode(const uint8_t);
        uint8_t get_runMode() const;
        void set_rideMode(const uint8_t);
        uint8_t get_rideMode() const;
        void set_name(const std::string&);
        const std::string& get_name() const;
        
        
        static CliSelectCharReq create(const uint8_t&, const uint8_t&, const uint8_t&, const std::string&);
        static CliSelectCharReq create(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint8_t charId;
        uint8_t runMode;
        uint8_t rideMode;
        std::string name;
};

}
}
