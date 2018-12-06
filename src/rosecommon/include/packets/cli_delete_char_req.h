#pragma once


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class CliDeleteCharReq : public CRosePacket {
    public:
        CliDeleteCharReq();
        CliDeleteCharReq(CRoseReader reader);
        CliDeleteCharReq(CliDeleteCharReq&&) = default;
        CliDeleteCharReq& operator=(CliDeleteCharReq&&) = default;
        ~CliDeleteCharReq() = default;
        
        static constexpr size_t size();
        
        
        
        void set_charId(const uint8_t);
        uint8_t get_charId() const;
        void set_isDelete(const uint8_t);
        uint8_t get_isDelete() const;
        void set_name(const std::string&);
        const std::string& get_name() const;
        
        
        static CliDeleteCharReq create(const uint8_t&, const uint8_t&, const std::string&);
        static CliDeleteCharReq create(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint8_t charId;
        uint8_t isDelete;
        std::string name;
};

}
}
