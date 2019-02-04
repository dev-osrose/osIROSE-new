#pragma once

/* Generated with IDL v0.1.0 */


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class SrvBillingMessage : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_BILLING_MESSAGE;
        SrvBillingMessage();
        SrvBillingMessage(CRoseReader reader);
        SrvBillingMessage(SrvBillingMessage&&) = default;
        SrvBillingMessage& operator=(SrvBillingMessage&&) = default;
        ~SrvBillingMessage() = default;
        
        static constexpr size_t size();
        
        
        
        void set_functionType(const uint16_t);
        uint16_t get_functionType() const;
        void set_payFlag(const uint32_t);
        uint32_t get_payFlag() const;
        
        
        static SrvBillingMessage create();
        static SrvBillingMessage create(const uint8_t*);
        static std::unique_ptr<SrvBillingMessage> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t functionType = 0x1001;
        uint32_t payFlag = 2;
};

}
}
