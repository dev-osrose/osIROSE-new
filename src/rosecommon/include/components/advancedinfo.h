#pragma once

struct AdvancedInfo {
    enum MoveMode {
        WALK = 0,
        RUN = 1,
        DRIVE = 2,
        RIDEON = 4
    };

    AdvancedInfo() : zuly_(0), hp_(0), mp_(0), moveMode_(AdvancedInfo::RUN), runSpeed_(0), atkSpeed_(0), weightRate_(0) {}

    uint32_t zuly_;
    uint32_t hp_;
    uint32_t mp_;
    uint8_t moveMode_;
    uint16_t runSpeed_;
    uint16_t atkSpeed_;
    uint8_t weightRate_;
};

