#pragma once


#include "packetfactory.h"
#include "dataconsts.h"
#include <string>
#include <array>

namespace RoseCommon {
namespace Packet {

class SrvPlayerChar : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_PLAYER_CHAR;
        SrvPlayerChar();
        SrvPlayerChar(CRoseReader reader);
        SrvPlayerChar(SrvPlayerChar&&) = default;
        SrvPlayerChar& operator=(SrvPlayerChar&&) = default;
        ~SrvPlayerChar() = default;
        
        static constexpr size_t size();
        
        
        struct EquippedItem : public ISerialize {
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            static constexpr size_t size();
            
            void set_id(const unsigned int);
            unsigned int get_id() const;
            void set_gem_opt(const unsigned int);
            unsigned int get_gem_opt() const;
            void set_socket(const unsigned int);
            unsigned int get_socket() const;
            void set_grade(const unsigned int);
            unsigned int get_grade() const;
            void set_data(const uint32_t);
            uint32_t get_data() const;
            
            private:
                union {
                    PACK(struct {
                        unsigned int id : 10;
                        unsigned int gem_opt : 9;
                        unsigned int socket : 1;
                        unsigned int grade : 4;
                    });
                    uint32_t data = 0;
                } data;
        };
        
        struct Header : public ISerialize {
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            static constexpr size_t size();
            
            void set_type(const unsigned int);
            unsigned int get_type() const;
            void set_id(const unsigned int);
            unsigned int get_id() const;
            void set_isCreated(const unsigned int);
            unsigned int get_isCreated() const;
            void set_header(const uint16_t);
            uint16_t get_header() const;
            
            private:
                union {
                    PACK(struct {
                        unsigned int type : 5;
                        unsigned int id : 10;
                        unsigned int isCreated : 1;
                    });
                    uint16_t header = 0;
                } data;
        };
        
        
        void set_id(const uint16_t);
        uint16_t get_id() const;
        void set_x(const float);
        float get_x() const;
        void set_y(const float);
        float get_y() const;
        void set_destX(const float);
        float get_destX() const;
        void set_destY(const float);
        float get_destY() const;
        void set_command(const uint16_t);
        uint16_t get_command() const;
        void set_targetId(const uint16_t);
        uint16_t get_targetId() const;
        void set_moveMode(const uint8_t);
        uint8_t get_moveMode() const;
        void set_hp(const int32_t);
        int32_t get_hp() const;
        void set_teamId(const int32_t);
        int32_t get_teamId() const;
        void set_statusFlag(const uint32_t);
        uint32_t get_statusFlag() const;
        void set_race(const uint8_t);
        uint8_t get_race() const;
        void set_runSpeed(const int16_t);
        int16_t get_runSpeed() const;
        void set_atkSpeed(const int16_t);
        int16_t get_atkSpeed() const;
        void set_weightRate(const uint8_t);
        uint8_t get_weightRate() const;
        void set_face(const uint32_t);
        uint32_t get_face() const;
        void set_hair(const uint32_t);
        uint32_t get_hair() const;
        void set_inventory(const std::array<EquippedItem, MAX_VISIBLE_ITEMS>&);
        void set_inventory(const EquippedItem&, size_t index);
        const std::array<EquippedItem, MAX_VISIBLE_ITEMS>& get_inventory() const;
        const EquippedItem& get_inventory(size_t index) const;
        void set_bullets(const std::array<Header, BulletType::MAX_BULLET_TYPES>&);
        void set_bullets(const Header&, size_t index);
        const std::array<Header, BulletType::MAX_BULLET_TYPES>& get_bullets() const;
        const Header& get_bullets(size_t index) const;
        void set_job(const int16_t);
        int16_t get_job() const;
        void set_level(const uint8_t);
        uint8_t get_level() const;
        void set_ridingItems(const std::array<EquippedItem, RidingItem::MAX_RIDING_ITEMS>&);
        void set_ridingItems(const EquippedItem&, size_t index);
        const std::array<EquippedItem, RidingItem::MAX_RIDING_ITEMS>& get_ridingItems() const;
        const EquippedItem& get_ridingItems(size_t index) const;
        void set_z(const int16_t);
        int16_t get_z() const;
        void set_subFlag(const uint32_t);
        uint32_t get_subFlag() const;
        void set_name(const std::string&);
        const std::string& get_name() const;
        void set_otherName(const std::string&);
        const std::string& get_otherName() const;
        
        
        static SrvPlayerChar create(const uint16_t&);
        static SrvPlayerChar create(const uint8_t*);
        static std::unique_ptr<SrvPlayerChar> allocate(const uint8_t*);
    
    protected:
        virtual bool pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t id;
        float x = 0;
        float y = 0;
        float destX = 0;
        float destY = 0;
        uint16_t command = 0;
        uint16_t targetId = 0;
        uint8_t moveMode = 0;
        int32_t hp = 0;
        int32_t teamId = 0;
        uint32_t statusFlag = 0;
        uint8_t race = 0;
        int16_t runSpeed = 0;
        int16_t atkSpeed = 0;
        uint8_t weightRate = 0;
        uint32_t face = 0;
        uint32_t hair = 0;
        std::array<EquippedItem, MAX_VISIBLE_ITEMS> inventory = {};
        std::array<Header, BulletType::MAX_BULLET_TYPES> bullets = {};
        int16_t job = 0;
        uint8_t level = 0;
        std::array<EquippedItem, RidingItem::MAX_RIDING_ITEMS> ridingItems = {};
        int16_t z = 0;
        uint32_t subFlag = 0;
        std::string name;
        std::string otherName;
};

}
}
