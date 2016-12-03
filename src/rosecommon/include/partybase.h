#pragma once

#include <vector>
#include <memory>

struct PartyBase {
    PartyBase() :
        xp_(0),
        range_(7), // FIXME : set that in a configurable way
        level_(1),
        options_(0),
        lastItem_(-1),
        lastEtc_(-1),
        lastZuly_(-1),
        capacity_(5) {} // FIXME : set that in a configurable way

    uint32_t xp_;
    int range_;
    uint8_t level_;
    Entity leader_;
    std::vector<Entity> members_;
    uint8_t options_;
    int lastItem_; // id of last person to receive an item
    int lastEtc_; // id of last person to receive an etc
    int lastZuly_; // id of last person to receive zuly
    uint8_t capacity_;
};

