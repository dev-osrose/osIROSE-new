#pragma once

#include <vector>
#include <memory>

struct PartyBase {
    PartyBase() :
        xp_(0),
        maxLevel_(0),
        level_(0),
        flag_(0),
        options_(0),
        lastItem_(-1),
        lastEtc_(-1),
        lastZuly_(-1),
        capacity_(0) {}

    uint32_t xp_;
    uint8_t maxLevel_;
    uint8_t level_;
    std::vector<Entity> members_;
    uint32_t flag_;
    uint32_t options_;
    int lastItem_; // id of last person to receive an item
    int lastEtc_; // id of last person to receive an etc
    int lastZuly_; // id of last person to receive zuly
    uint8_t capacity_;
};

