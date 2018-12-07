#pragma once


#include "packetfactory.h"
#include "dataconsts.h"

namespace RoseCommon {
namespace Packet {

class SrvInventoryData : public CRosePacket {
    public:
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
                    uint16_t header;
                } data;
        };
        
        
        void set_zuly(const int64_t);
        int64_t get_zuly() const;
        void set_items(const Item*);
        void set_items(const Item, size_t index);
        const Item* get_items() const;
        Item get_items(size_t index) const;
        
        
        static SrvInventoryData create(const int64_t&);
        static SrvInventoryData create(const uint8_t*);
        static std::unique_ptr<SrvInventoryData> allocate(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        int64_t zuly;
        Item items[MAX_ITEMS];
};

}
}
