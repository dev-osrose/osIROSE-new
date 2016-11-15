#pragma once

#include <array>
#include <initializer_list>
#include "crosepacket.h"

// This is not a component!
struct HotbarItem : public RoseCommon::ISerialize {
    HotbarItem() : item_(0) {}
    HotbarItem(uint16_t item) : item_(item) {}
    HotbarItem(uint8_t type, uint16_t slotId) : type_(type), slotId_(slotId) {}
    virtual ~HotbarItem() {}

    union {
        uint16_t item_;
        struct {
            uint8_t type_ : 5;
            uint16_t slotId_ : 11;
        };
    };

    protected:
        virtual void serialize(RoseCommon::CRosePacket &os) const {
            os << item_;
        }
        virtual void deserialize(RoseCommon::CRosePacket&) {}
};

struct Hotbar {
    static const uint16_t maxItems = 32;

    Hotbar() {}
    Hotbar(const std::initializer_list<HotbarItem> &items) {
        int64_t diff = maxItems - items.size();
        diff = diff < 0 ? 0 : diff;
        auto last = items.end() - diff;
        std::copy(items.begin(), last, items_.begin());
    }

    std::array<HotbarItem, maxItems> items_;
};

