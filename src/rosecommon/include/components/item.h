#pragma once

#include "crosepacket.h"

// This is not a component!
struct Item : public RoseCommon::ISerialize {
    Item() : type_(WEARABLE), runSpeed_(0), atkSpeed_(0) {}
    virtual ~Item() {}

    enum ItemType {
        WEARABLE = 0,
        CONSUMABLE = 1,
        ETC = 2,
        RIDING = 3,
        MAX_INV_TYPE
    };

    PACK(union {
        // Wearable
        PACK(struct {
            uint8_t type_ : 5;
            uint16_t id_ : 10;
            bool isCreated_;
            uint16_t gemOpt_ : 9;
            uint8_t durability_ : 7;
            uint16_t life_ : 10;
            bool hasSocket_;
            bool isAppraisal_;
            uint8_t refine_ : 4;
        }) wearable_ {};
        // CONSUMABLE
        PACK(struct {
            uint8_t type_ : 5;
            uint16_t id_ : 10;
            uint32_t quantity_;
        }) consumable_;
        // Money
        PACK(struct {
            uint8_t type_ : 5;
            uint16_t reserved_ : 11;
            uint32_t zuly_;
        }) money_;
        // Bullet
        PACK(struct {
            PACK(union {
                PACK(struct {
                    uint8_t type_ : 5;
                    uint16_t id_ : 10;
                });
                uint16_t item_;
            });
        }) bullet_;
        PACK(struct {
            uint16_t totalPart1_;
            uint32_t totalPart2_;
        });
    });

    ItemType type_;

    float runSpeed_;
    float atkSpeed_;

    public:
        void partialSerialize(RoseCommon::CRosePacket &os) const {
            uint32_t data = (wearable_.refine_ << 20) | (wearable_.hasSocket_ << 19) |
                            (wearable_.gemOpt_ << 10) | wearable_.id_;
            os << data;
        }
        void bulletPartialSerialize(RoseCommon::CRosePacket &os) const {
            os << bullet_.item_;
        }
    protected:
        virtual void serialize(RoseCommon::CRosePacket &os) const {
            os << totalPart1_ << totalPart2_;
        }
        virtual void deserialize(RoseCommon::CRosePacket&) {}
};

