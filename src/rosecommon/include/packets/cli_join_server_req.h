#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class CliJoinServerReq : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_JOIN_SERVER_REQ;
        CliJoinServerReq();
        CliJoinServerReq(CRoseReader reader);
        CliJoinServerReq(CliJoinServerReq&&) = default;
        CliJoinServerReq& operator=(CliJoinServerReq&&) = default;
        ~CliJoinServerReq() = default;
        
        static constexpr size_t size();
        
        
        struct Password : public ISerialize {
            Password();
            Password(std::string);
            Password(const Password&) = default;
            Password(Password&&) = default;
            Password& operator=(const Password&) = default;
            Password& operator=(Password&&) = default;
            virtual ~Password() = default;
            
            static constexpr size_t size();
            
            operator std::string() const { return password; }
            bool isValid() const { return is_valid; }
            
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            private:
                std::string password;
                bool is_valid;
        };
        
        
        void set_sessionId(const uint32_t);
        uint32_t get_sessionId() const;
        void set_password(const Password&);
        const Password& get_password() const;
        
        
        static CliJoinServerReq create(const uint32_t&, const Password&);
        static CliJoinServerReq create(const uint8_t*);
        static std::unique_ptr<CliJoinServerReq> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint32_t sessionId;
        Password password;
};

}
}
