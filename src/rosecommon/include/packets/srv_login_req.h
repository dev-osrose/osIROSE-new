#pragma once


#include "packetfactory.h"
#include <string>
#include <vector>

namespace RoseCommon {
namespace Packet {

class SrvLoginReq : public CRosePacket {
    public:
        SrvLoginReq();
        SrvLoginReq(CRoseReader reader);
        SrvLoginReq(SrvLoginReq&&) = default;
        SrvLoginReq& operator=(SrvLoginReq&&) = default;
        ~SrvLoginReq() = default;
        
        static constexpr size_t size();
        
        
        struct ChannelInfo : public ISerialize {
            virtual bool read(CRoseReader&) override;
            virtual bool write(CRoseBasePolicy&) const override;
            
            static constexpr size_t size();
            
            void set_id(const uint8_t);
            uint8_t get_id() const;
            void set_lowAge(const uint8_t);
            uint8_t get_lowAge() const;
            void set_highAge(const uint8_t);
            uint8_t get_highAge() const;
            void set_capacity(const uint16_t);
            uint16_t get_capacity() const;
            void set_name(const std::string&);
            const std::string& get_name() const;
            
            private:
                uint8_t id;
                uint8_t lowAge;
                uint8_t highAge;
                uint16_t capacity;
                std::string name;
        };
        
        
        void set_id(const uint32_t);
        uint32_t get_id() const;
        void set_channels(const std::vector<ChannelInfo>&);
        void add_channels(const ChannelInfo&);
        const std::vector<ChannelInfo>& get_channels() const;
        const ChannelInfo& get_channels(size_t index) const;
        
        
        static SrvLoginReq create(const uint32_t&);
        static SrvLoginReq create(const uint8_t*);
    
    protected:
        virtual void pack(CRoseBasePolicy&) const override;
    
    private:
        uint32_t id;
        std::vector<ChannelInfo> channels;
};

}
}
