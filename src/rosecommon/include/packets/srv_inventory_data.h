#pragma once


#include "packetfactory.h"
#include "dataconsts.h"
#include <array>

namespace RoseCommon {
namespace Packet {

class SrvInventoryData : public CRosePacket {
    public:
        static constexpr ePacketType PACKET_ID = ePacketType::PAKWC_INVENTORY_DATA;
        SrvInventoryData();
        SrvInventoryData(CRoseReader reader);
        SrvInventoryData(SrvInventoryData&&) = default;
        SrvInventoryData& operator=(SrvInventoryData&&) = default;
        ~SrvInventoryData() = default;
        
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
        
        
        void set_zuly(const int64_t);
        int64_t get_zuly() const;
        void set_items(const std::array<Item, MAX_ITEMS>);
        void set_items(const Item, size_t index);
        std::array<Item, MAX_ITEMS> get_items() const;
        Item get_items(size_t index) const;
        
        
        static SrvInventoryData create(const int64_t&);
        static SrvInventoryData create(const uint8_t*);
        static std::unique_ptr<SrvInventoryData> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        int64_t zuly;
        std::array<Item, MAX_ITEMS> items = {};
};

}
}
