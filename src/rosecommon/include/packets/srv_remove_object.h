#pragma once


#include "packetfactory.h"

namespace RoseCommon {
namespace Packet {

class SrvRemoveObject : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_REMOVE_OBJECT;
        SrvRemoveObject();
        SrvRemoveObject(CRoseReader reader);
        SrvRemoveObject(SrvRemoveObject&&) = default;
        SrvRemoveObject& operator=(SrvRemoveObject&&) = default;
        ~SrvRemoveObject() = default;
        
        static constexpr size_t size();
        
        
        
        void set_id(const uint16_t);
        uint16_t get_id() const;
        
        
        static SrvRemoveObject create(const uint16_t&);
        static SrvRemoveObject create(const uint8_t*);
        static std::unique_ptr<SrvRemoveObject> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t id;
};

}
}
