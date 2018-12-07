#pragma once


#include "packetfactory.h"
#include <string>

namespace RoseCommon {
namespace Packet {

class SrvSrvSelectReply : public CRosePacket {
    public:
        SrvSrvSelectReply();
        SrvSrvSelectReply(CRoseReader reader);
        SrvSrvSelectReply(SrvSrvSelectReply&&) = default;
        SrvSrvSelectReply& operator=(SrvSrvSelectReply&&) = default;
        ~SrvSrvSelectReply() = default;
        
        static constexpr size_t size();
        
        
        enum Result : uint8_t {
            OK = 0,
            FAILED = 1,
            FULL = 2,
            INVALID_CHANNEL = 3,
            CHANNEL_NOT_ACTIVE = 4,
            INVALID_AGE = 5,
        };
        
        
        void set_result(const Result);
        Result get_result() const;
        void set_sessionId(const uint32_t);
        uint32_t get_sessionId() const;
        void set_cryptVal(const uint32_t);
        uint32_t get_cryptVal() const;
        void set_ip(const std::string&);
        const std::string& get_ip() const;
        void set_port(const uint16_t);
        uint16_t get_port() const;
        
        
        static SrvSrvSelectReply create(const Result&, const uint32_t&, const uint32_t&, const std::string&, const uint16_t&);
        static SrvSrvSelectReply create(const uint8_t*);
        static std::unique_ptr<SrvSrvSelectReply> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        Result result;
        uint32_t sessionId;
        uint32_t cryptVal;
        std::string ip;
        uint16_t port;
};

}
}
