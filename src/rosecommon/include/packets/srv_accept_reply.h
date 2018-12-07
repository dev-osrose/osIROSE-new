#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class SrvAcceptReply : public CRosePacket {
    public:
        SrvAcceptReply();
        SrvAcceptReply(CRoseReader reader);
        SrvAcceptReply(SrvAcceptReply&&) = default;
        SrvAcceptReply& operator=(SrvAcceptReply&&) = default;
        ~SrvAcceptReply() = default;
        
        static constexpr size_t size();
        
        
        
        void set_result(const uint8_t);
        uint8_t get_result() const;
        void set_randValue(const uint32_t);
        uint32_t get_randValue() const;
        
        
        static SrvAcceptReply create(const uint8_t&, const uint32_t&);
        static SrvAcceptReply create(const uint8_t*);
        static std::unique_ptr<SrvAcceptReply> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint8_t result;
        uint32_t randValue;
};

}
}
