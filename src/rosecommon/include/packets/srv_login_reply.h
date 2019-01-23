#pragma once


#include "packetfactory.h"
#include <string>
#include <vector>

namespace RoseCommon {
namespace Packet {

class SrvLoginReply : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKLC_LOGIN_REPLY;
        SrvLoginReply();
        SrvLoginReply(CRoseReader reader);
        SrvLoginReply(SrvLoginReply&&) = default;
        SrvLoginReply& operator=(SrvLoginReply&&) = default;
        ~SrvLoginReply() = default;
        
        static constexpr size_t size();
        
        
        enum Result : uint8_t {
            OK = 0,
            FAILED = 1,
            UNKNOWN_ACCOUNT = 2,
            INVALID_PASSWORD = 3,
            ALREADY_LOGGEDIN = 4,
            REFUSED_ACCOUNT = 5,
            NEED_CHARGE = 6,
            NO_RIGHT_TO_CONNECT = 7,
            TOO_MANY_USERS = 8,
            NO_NAME = 9,
            INVALID_VERSION = 10,
            OUTSIDE_REGION = 11,
        };
        
        struct ServerInfo : public ISerialize {
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            static constexpr size_t size();
            
            void set_test(const char);
            char get_test() const;
            void set_name(const std::string&);
            const std::string& get_name() const;
            void set_id(const uint32_t);
            uint32_t get_id() const;
            
            private:
                char test;
                std::string name;
                uint32_t id;
        };
        
        
        void set_result(const Result);
        Result get_result() const;
        void set_right(const uint16_t);
        uint16_t get_right() const;
        void set_type(const uint16_t);
        uint16_t get_type() const;
        void set_serversInfo(const std::vector<ServerInfo>&);
        void add_serversInfo(const ServerInfo&);
        const std::vector<ServerInfo>& get_serversInfo() const;
        const ServerInfo& get_serversInfo(size_t index) const;
        
        
        static SrvLoginReply create(const Result&, const uint16_t&, const uint16_t&);
        static SrvLoginReply create(const uint8_t*);
        static std::unique_ptr<SrvLoginReply> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        Result result;
        uint16_t right;
        uint16_t type;
        std::vector<ServerInfo> serversInfo;
};

}
}
