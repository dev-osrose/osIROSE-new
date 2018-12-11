#pragma once


#include "packetfactory.h"
#include "dataconsts.h"
#include <string>
#include <array>

namespace RoseCommon {
namespace Packet {

class SrvSelectCharReply : public CRosePacket {
    public:
        SrvSelectCharReply();
        SrvSelectCharReply(CRoseReader reader);
        SrvSelectCharReply(SrvSelectCharReply&&) = default;
        SrvSelectCharReply& operator=(SrvSelectCharReply&&) = default;
        ~SrvSelectCharReply() = default;
        
        static constexpr size_t size();
        
        
        enum EquippedPosition : uint8_t {
            HELMET = 0,
            ARMOR = 1,
            GAUNTLET = 2,
            BOOTS = 3,
            GOOGLES = 4,
            BACKPACK = 5,
            WEAPON_R = 6,
            WEAPON_L = 7,
            MAX_ITEMS = 8,
        };
        
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
                    uint32_t data;
                } data;
        };
        
        
        void set_race(const uint8_t);
        uint8_t get_race() const;
        void set_map(const uint16_t);
        uint16_t get_map() const;
        void set_x(const float);
        float get_x() const;
        void set_y(const float);
        float get_y() const;
        void set_spawn(const uint16_t);
        uint16_t get_spawn() const;
        void set_mask(const uint32_t);
        uint32_t get_mask() const;
        void set_headGear(const uint32_t);
        uint32_t get_headGear() const;
        void set_inventory(const std::array<EquippedItem, EquippedPosition::MAX_ITEMS>&);
        void set_inventory(const EquippedItem&, size_t index);
        const std::array<EquippedItem, EquippedPosition::MAX_ITEMS>& get_inventory() const;
        const EquippedItem& get_inventory(size_t index) const;
        void set_stone(const uint8_t);
        uint8_t get_stone() const;
        void set_face(const uint8_t);
        uint8_t get_face() const;
        void set_hair(const uint8_t);
        uint8_t get_hair() const;
        void set_job(const uint16_t);
        uint16_t get_job() const;
        void set_factionId(const uint8_t);
        uint8_t get_factionId() const;
        void set_factionRank(const uint8_t);
        uint8_t get_factionRank() const;
        void set_fame(const uint8_t);
        uint8_t get_fame() const;
        void set_str(const uint16_t);
        uint16_t get_str() const;
        void set_dex(const uint16_t);
        uint16_t get_dex() const;
        void set_int_(const uint16_t);
        uint16_t get_int_() const;
        void set_con(const uint16_t);
        uint16_t get_con() const;
        void set_charm(const uint16_t);
        uint16_t get_charm() const;
        void set_sense(const uint16_t);
        uint16_t get_sense() const;
        void set_hp(const int32_t);
        int32_t get_hp() const;
        void set_mp(const int32_t);
        int32_t get_mp() const;
        void set_xp(const uint32_t);
        uint32_t get_xp() const;
        void set_level(const uint16_t);
        uint16_t get_level() const;
        void set_statPoints(const uint32_t);
        uint32_t get_statPoints() const;
        void set_skillPoints(const uint32_t);
        uint32_t get_skillPoints() const;
        void set_bodySize(const uint8_t);
        uint8_t get_bodySize() const;
        void set_headSize(const uint8_t);
        uint8_t get_headSize() const;
        void set_penaltyXp(const uint32_t);
        uint32_t get_penaltyXp() const;
        void set_factionFame(const std::array<uint16_t, 2>&);
        void set_factionFame(const uint16_t&, size_t index);
        const std::array<uint16_t, 2>& get_factionFame() const;
        const uint16_t& get_factionFame(size_t index) const;
        void set_factionPoints(const std::array<uint16_t, 10>&);
        void set_factionPoints(const uint16_t&, size_t index);
        const std::array<uint16_t, 10>& get_factionPoints() const;
        const uint16_t& get_factionPoints(size_t index) const;
        void set_guildId(const uint32_t);
        uint32_t get_guildId() const;
        void set_guildContribution(const uint16_t);
        uint16_t get_guildContribution() const;
        void set_guildRank(const uint8_t);
        uint8_t get_guildRank() const;
        void set_pkFlag(const uint16_t);
        uint16_t get_pkFlag() const;
        void set_stamina(const uint16_t);
        uint16_t get_stamina() const;
        void set_effects(const std::array<StatusEffect, MAX_STATUS_EFFECTS>&);
        void set_effects(const StatusEffect&, size_t index);
        const std::array<StatusEffect, MAX_STATUS_EFFECTS>& get_effects() const;
        const StatusEffect& get_effects(size_t index) const;
        void set_patHp(const uint16_t);
        uint16_t get_patHp() const;
        void set_patCooldownTime(const uint32_t);
        uint32_t get_patCooldownTime() const;
        void set_skills(const std::array<uint16_t, MAX_SKILL_COUNT>&);
        void set_skills(const uint16_t&, size_t index);
        const std::array<uint16_t, MAX_SKILL_COUNT>& get_skills() const;
        const uint16_t& get_skills(size_t index) const;
        void set_hotbar(const std::array<HotbarItem, MAX_HOTBAR_ITEMS>&);
        void set_hotbar(const HotbarItem&, size_t index);
        const std::array<HotbarItem, MAX_HOTBAR_ITEMS>& get_hotbar() const;
        const HotbarItem& get_hotbar(size_t index) const;
        void set_tag(const uint32_t);
        uint32_t get_tag() const;
        void set_name(const std::string&);
        const std::string& get_name() const;
        
        
        static SrvSelectCharReply create();
        static SrvSelectCharReply create(const uint8_t*);
        static std::unique_ptr<SrvSelectCharReply> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint8_t race;
        uint16_t map;
        float x;
        float y;
        uint16_t spawn;
        uint32_t mask;
        uint32_t headGear;
        std::array<EquippedItem, EquippedPosition::MAX_ITEMS> inventory;
        uint8_t stone;
        uint8_t face;
        uint8_t hair;
        uint16_t job;
        uint8_t factionId = 0;
        uint8_t factionRank = 0;
        uint8_t fame = 0;
        uint16_t str;
        uint16_t dex;
        uint16_t int_;
        uint16_t con;
        uint16_t charm;
        uint16_t sense;
        int32_t hp;
        int32_t mp;
        uint32_t xp;
        uint16_t level;
        uint32_t statPoints;
        uint32_t skillPoints;
        uint8_t bodySize;
        uint8_t headSize;
        uint32_t penaltyXp;
        std::array<uint16_t, 2> factionFame = {0};
        std::array<uint16_t, 10> factionPoints = {0};
        uint32_t guildId = 0;
        uint16_t guildContribution = 0;
        uint8_t guildRank = 0;
        uint16_t pkFlag;
        uint16_t stamina;
        std::array<StatusEffect, MAX_STATUS_EFFECTS> effects = {};
        uint16_t patHp = 0;
        uint32_t patCooldownTime = 0;
        std::array<uint16_t, MAX_SKILL_COUNT> skills = {0};
        std::array<HotbarItem, MAX_HOTBAR_ITEMS> hotbar = {};
        uint32_t tag;
        std::string name;
};

}
}
