#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class SrvCreateCharReply : public CRosePacket {
    public:
        SrvCreateCharReply();
        SrvCreateCharReply(CRoseReader reader);
        SrvCreateCharReply(SrvCreateCharReply&&) = default;
        SrvCreateCharReply& operator=(SrvCreateCharReply&&) = default;
        ~SrvCreateCharReply() = default;
        
        static constexpr size_t size();
        
        
        enum Result : uint8_t {
            OK = 0,
            FAILED = 1,
            NAME_TAKEN = 2,
            INVALID_VALUE = 3,
            TOO_MANY_CHARS = 4,
            BLOCKED = 5,
        };
        
        
        void set_result(const Result);
        Result get_result() const;
        void set_platininum(const uint8_t);
        uint8_t get_platininum() const;
        
        
        static SrvCreateCharReply create(const Result&);
        static SrvCreateCharReply create(const uint8_t*);
        static std::unique_ptr<SrvCreateCharReply> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        Result result;
        uint8_t platininum = 0;
};

}
}
