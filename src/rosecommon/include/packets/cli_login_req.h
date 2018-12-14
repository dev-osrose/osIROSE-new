#pragma once


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class CliLoginReq : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKCS_LOGIN_REQ;
        CliLoginReq();
        CliLoginReq(CRoseReader reader);
        CliLoginReq(CliLoginReq&&) = default;
        CliLoginReq& operator=(CliLoginReq&&) = default;
        ~CliLoginReq() = default;
        
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
        
        
        void set_password(const Password&);
        const Password& get_password() const;
        void set_username(const std::string&);
        const std::string& get_username() const;
        
        
        static CliLoginReq create(const Password&, const std::string&);
        static CliLoginReq create(const uint8_t*);
        static std::unique_ptr<CliLoginReq> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        Password password;
        std::string username;
};

}
}
