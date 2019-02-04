#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class SrvJoinServerReply : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKSC_JOIN_SERVER_REPLY;
        SrvJoinServerReply();
        SrvJoinServerReply(CRoseReader reader);
        SrvJoinServerReply(SrvJoinServerReply&&) = default;
        SrvJoinServerReply& operator=(SrvJoinServerReply&&) = default;
        ~SrvJoinServerReply() = default;
        
        static constexpr size_t size();
        
        
        enum Result : uint8_t {
            OK = 0,
            FAILED = 1,
            TIME_OUT = 2,
            INVALID_PASSWORD = 3,
            ALREADY_LOGGEDIN = 4,
        };
        
        
        void set_result(const Result);
        Result get_result() const;
        void set_id(const uint32_t);
        uint32_t get_id() const;
        void set_payFlag(const uint32_t);
        uint32_t get_payFlag() const;
        
        
        static SrvJoinServerReply create(const Result&, const uint32_t&);
        static SrvJoinServerReply create(const uint8_t*);
        static std::unique_ptr<SrvJoinServerReply> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        Result result;
        uint32_t id;
        uint32_t payFlag = 0;
};

}
}
