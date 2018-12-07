#pragma once


#include "packetfactory.h"
#include <string>
#include "isccommon.h"

namespace RoseCommon {
namespace Packet {

class IscServerRegister : public CRosePacket {
    public:
        IscServerRegister();
        IscServerRegister(CRoseReader reader);
        IscServerRegister(IscServerRegister&&) = default;
        IscServerRegister& operator=(IscServerRegister&&) = default;
        ~IscServerRegister() = default;
        
        static constexpr size_t size();
        
        
        
        void set_serverType(const Isc::ServerType);
        Isc::ServerType get_serverType() const;
        void set_name(const std::string&);
        const std::string& get_name() const;
        void set_addr(const std::string&);
        const std::string& get_addr() const;
        void set_port(const int32_t);
        int32_t get_port() const;
        void set_right(const int32_t);
        int32_t get_right() const;
        void set_id(const int32_t);
        int32_t get_id() const;
        
        
        static IscServerRegister create(const Isc::ServerType&, const std::string&, const std::string&, const int32_t&, const int32_t&, const int32_t&);
        static IscServerRegister create(const uint8_t*);
        static std::unique_ptr<IscServerRegister> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        Isc::ServerType serverType;
        std::string name;
        std::string addr;
        int32_t port;
        int32_t right;
        int32_t id;
};

}
}
