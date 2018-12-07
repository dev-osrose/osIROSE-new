#pragma once


#include "packetfactory.h"
#include "dataconsts.h"

namespace RoseCommon {
namespace Packet {

class SrvQuestData : public CRosePacket {
    public:
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
                    uint16_t header;
                } data;
        };
        
        struct Quest : public ISerialize {
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            static constexpr size_t size();
            
            void set_id(const uint16_t);
            uint16_t get_id() const;
            void set_timer(const uint32_t);
            uint32_t get_timer() const;
            void set_vars(const uint32_t*);
            void set_vars(const uint32_t, size_t index);
            const uint32_t* get_vars() const;
            uint32_t get_vars(size_t index) const;
            void set_switches(const uint32_t);
            uint32_t get_switches() const;
            void set_items(const Item*);
            void set_items(const Item, size_t index);
            const Item* get_items() const;
            Item get_items(size_t index) const;
            
            private:
                uint16_t id = 0;
                // timer: Unlimited if 0
                uint32_t timer = 0;
                uint32_t vars[MAX_QUEST_VARS];
                uint32_t switches = 0;
                Item items[MAX_QUEST_ITEMS];
        };
        
        
        void set_episode(const uint16_t*);
        void set_episode(const uint16_t, size_t index);
        const uint16_t* get_episode() const;
        uint16_t get_episode(size_t index) const;
        void set_job(const uint16_t*);
        void set_job(const uint16_t, size_t index);
        const uint16_t* get_job() const;
        uint16_t get_job(size_t index) const;
        void set_planet(const uint16_t*);
        void set_planet(const uint16_t, size_t index);
        const uint16_t* get_planet() const;
        uint16_t get_planet(size_t index) const;
        void set_union(const uint16_t*);
        void set_union(const uint16_t, size_t index);
        const uint16_t* get_union() const;
        uint16_t get_union(size_t index) const;
        void set_quests(const Quest*);
        void set_quests(const Quest, size_t index);
        const Quest* get_quests() const;
        Quest get_quests(size_t index) const;
        void set_switches(const uint32_t*);
        void set_switches(const uint32_t, size_t index);
        const uint32_t* get_switches() const;
        uint32_t get_switches(size_t index) const;
        
        
        static SrvQuestData create();
        static SrvQuestData create(const uint8_t*);
        static std::unique_ptr<SrvQuestData> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint16_t episode[MAX_CONDITIONS_EPISODE];
        uint16_t job[MAX_CONDTIONS_JOB];
        uint16_t planet[MAX_CONDITIONS_PLANET];
        uint16_t union[MAX_CONDITIONS_UNION];
        Quest quests[MAX_QUESTS];
        uint32_t switches[MAX_SWITCHES];
};

}
}
