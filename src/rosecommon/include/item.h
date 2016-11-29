#pragma once
#include "iserialize.h"

namespace RoseCommon {

struct Item : public ISerialize {
    enum Type {
        WEARABLE = 0,
        CONSUMABLE = 1,
        ETC = 2,
        RIDING = 3
    };

    Item();
    virtual ~Item() = default;

    virtual uint32_t getVisible() const override;
    virtual uint16_t getHeader() const override;
    virtual uint32_t getData() const override;

    virtual void saveToDatabase() const override;
    virtual void loadFromDatabase(Core::IRow&) override;

    uint8_t type_;
    uint16_t id_;
    bool isCreated_;
    uint16_t gemOpt_;
    uint8_t durability_;
    uint16_t life_;
    bool hasSocket_;
    bool isAppraised_;
    uint8_t refine_;
    uint32_t count_;
    bool isStackable_;
};

}
