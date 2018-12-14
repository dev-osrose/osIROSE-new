#pragma once


#include "packetfactory.h"
#include "dataconsts.h"
#include <array>

namespace RoseCommon {
namespace Packet {

class SrvQuestData : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_QUEST_DATA;
        SrvQuestData();
        SrvQuestData(CRoseReader reader);
        SrvQuestData(SrvQuestData&&) = default;
        SrvQuestData& operator=(SrvQuestData&&) = default;
        ~SrvQuestData() = default;
        
        static constexpr size_t size();
        
        
        struct Header : public ISerialize {
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            static constexpr size_t size();
            
            void set_isCreated(const unsigned int);
            unsigned int get_isCreated() const;
            void set_id(const unsigned int);
            unsigned int get_id() const;
            void set_type(const unsigned int);
            unsigned int get_type() const;
            void set_header(const uint16_t);
            uint16_t get_header() const;
            
            private:
                union {
                    PACK(struct {
                        unsigned int isCreated : 1;
                        unsigned int id : 10;
                        unsigned int type : 5;
                    });
                    uint16_t header = 0;
                } data;
        };
        
        struct Data : public ISerialize {
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            static constexpr size_t size();
            
            void set_refine(const unsigned int);
            unsigned int get_refine() const;
            void set_isAppraised(const unsigned int);
            unsigned int get_isAppraised() const;
            void set_hasSocket(const unsigned int);
            unsigned int get_hasSocket() const;
            void set_life(const unsigned int);
            unsigned int get_life() const;
            void set_durability(const unsigned int);
            unsigned int get_durability() const;
            void set_gem_opt(const unsigned int);
            unsigned int get_gem_opt() const;
            void set_count(const uint32_t);
            uint32_t get_count() const;
            
            private:
                union {
                    PACK(struct {
                        unsigned int refine : 4;
                        unsigned int isAppraised : 1;
                        unsigned int hasSocket : 1;
                        unsigned int life : 10;
                        unsigned int durability : 7;
                        unsigned int gem_opt : 9;
                    });
                    uint32_t count = 0;
                } data;
        };
        
        struct Item : public ISerialize {
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            static constexpr size_t size();
            
            void set_header(const Header);
            Header get_header() const;
            void set_data(const Data);
            Data get_data() const;
            
            private:
                Header header = {};
                Data data = {};
        };
        
        struct Quest : public ISerialize {
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            static constexpr size_t size();
            
            void set_id(const uint16_t);
            uint16_t get_id() const;
            void set_timer(const uint32_t);
            uint32_t get_timer() const;
            void set_vars(const std::array<uint32_t, MAX_QUEST_VARS>);
            void set_vars(const uint32_t, size_t index);
            std::array<uint32_t, MAX_QUEST_VARS> get_vars() const;
            uint32_t get_vars(size_t index) const;
            void set_switches(const uint32_t);
            uint32_t get_switches() const;
            void set_items(const std::array<Item, MAX_QUEST_ITEMS>);
            void set_items(const Item, size_t index);
            std::array<Item, MAX_QUEST_ITEMS> get_items() const;
            Item get_items(size_t index) const;
            
            private:
                uint16_t id = 0;
                // timer: Unlimited if 0
                uint32_t timer = 0;
                std::array<uint32_t, MAX_QUEST_VARS> vars = {0};
                uint32_t switches = 0;
                std::array<Item, MAX_QUEST_ITEMS> items = {};
        };
        
        
        void set_episodes(const std::array<uint16_t, MAX_CONDITIONS_EPISODE>);
        void set_episodes(const uint16_t, size_t index);
        std::array<uint16_t, MAX_CONDITIONS_EPISODE> get_episodes() const;
        uint16_t get_episodes(size_t index) const;
        void set_jobs(const std::array<uint16_t, MAX_CONDITIONS_JOB>);
        void set_jobs(const uint16_t, size_t index);
        std::array<uint16_t, MAX_CONDITIONS_JOB> get_jobs() const;
        uint16_t get_jobs(size_t index) const;
        void set_planets(const std::array<uint16_t, MAX_CONDITIONS_PLANET>);
        void set_planets(const uint16_t, size_t index);
        std::array<uint16_t, MAX_CONDITIONS_PLANET> get_planets() const;
        uint16_t get_planets(size_t index) const;
        void set_unions(const std::array<uint16_t, MAX_CONDITIONS_UNION>);
        void set_unions(const uint16_t, size_t index);
        std::array<uint16_t, MAX_CONDITIONS_UNION> get_unions() const;
        uint16_t get_unions(size_t index) const;
        void set_quests(const std::array<Quest, MAX_QUESTS>);
        void set_quests(const Quest, size_t index);
        std::array<Quest, MAX_QUESTS> get_quests() const;
        Quest get_quests(size_t index) const;
        void set_switches(const std::array<uint32_t, MAX_SWITCHES>);
        void set_switches(const uint32_t, size_t index);
        std::array<uint32_t, MAX_SWITCHES> get_switches() const;
        uint32_t get_switches(size_t index) const;
        
        
        static SrvQuestData create();
        static SrvQuestData create(const uint8_t*);
        static std::unique_ptr<SrvQuestData> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        std::array<uint16_t, MAX_CONDITIONS_EPISODE> episodes = {0};
        std::array<uint16_t, MAX_CONDITIONS_JOB> jobs = {0};
        std::array<uint16_t, MAX_CONDITIONS_PLANET> planets = {0};
        std::array<uint16_t, MAX_CONDITIONS_UNION> unions = {0};
        std::array<Quest, MAX_QUESTS> quests = {};
        std::array<uint32_t, MAX_SWITCHES> switches = {0};
};

}
}
