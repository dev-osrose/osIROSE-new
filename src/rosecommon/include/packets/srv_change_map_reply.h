#pragma once


#include "packetfactory.h"
#include "dataconsts.h"
#include <array>

namespace RoseCommon {
namespace Packet {

class SrvChangeMapReply : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_CHANGE_MAP_REPLY;
        SrvChangeMapReply();
        SrvChangeMapReply(CRoseReader reader);
        SrvChangeMapReply(SrvChangeMapReply&&) = default;
        SrvChangeMapReply& operator=(SrvChangeMapReply&&) = default;
        ~SrvChangeMapReply() = default;
        
        static constexpr size_t size();
        
        
        
        void set_objectIndex(const uint16_t);
        uint16_t get_objectIndex() const;
        void set_hp(const uint16_t);
        uint16_t get_hp() const;
        void set_mp(const uint16_t);
        uint16_t get_mp() const;
        void set_xp(const uint16_t);
        uint16_t get_xp() const;
        void set_penalizeXp(const uint16_t);
        uint16_t get_penalizeXp() const;
        void set_craftRate(const uint16_t);
        uint16_t get_craftRate() const;
        void set_updateTime(const uint16_t);
        uint16_t get_updateTime() const;
        void set_worldRate(const uint16_t);
        uint16_t get_worldRate() const;
        void set_townRate(const uint8_t);
        uint8_t get_townRate() const;
        void set_itemRate(const std::array<uint8_t, MAX_SELL_TYPE>);
        void set_itemRate(const uint8_t, size_t index);
        std::array<uint8_t, MAX_SELL_TYPE> get_itemRate() const;
        uint8_t get_itemRate(size_t index) const;
        void set_flags(const uint32_t);
        uint32_t get_flags() const;
        void set_worldTime(const uint16_t);
        uint16_t get_worldTime() const;
        void set_teamNumber(const uint16_t);
        uint16_t get_teamNumber() const;
        
        
        static SrvChangeMapReply create(const uint16_t&, const uint16_t&, const uint16_t&, const uint16_t&, const uint16_t&, const uint16_t&, const uint16_t&);
        static SrvChangeMapReply create(const uint8_t*);
        static std::unique_ptr<SrvChangeMapReply> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t objectIndex;
        uint16_t hp;
        uint16_t mp;
        uint16_t xp;
        uint16_t penalizeXp;
        uint16_t craftRate = 0;
        uint16_t updateTime = 0;
        uint16_t worldRate = 0;
        uint8_t townRate = 0;
        std::array<uint8_t, MAX_SELL_TYPE> itemRate = {0};
        uint32_t flags = 0;
        uint16_t worldTime;
        uint16_t teamNumber;
};

}
}
