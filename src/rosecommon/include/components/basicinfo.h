#pragma once

#include <string>
#include <atomic>

struct BasicInfo {
    enum Cmd {
        STOP = 0,
        MOVE = 1,
        ATTACK = 2,
        DIE = 3,
        PICKUP = 4,
        SKILL2SELF = 6,
        SKILL2OBJ = 7,
        SKILL2POS = 8,
        RUNAWAY = 0x8009,
        SIT = 10
    };

    BasicInfo() : level_(0), xp_(0), id_(0), tag_(0), teamId_(0), targetId_(0), command_(STOP), isOnMap_(false), ownerId_(0) {}
    template <typename T>
    BasicInfo(const T& row) : BasicInfo() {
        loadFromRow(row);
    }

    template <typename T>
    BasicInfo(const T& row, uint32_t id) : BasicInfo(row) {
        id_ = id;
        tag_ = id;
    }

    template <typename T>
    void loadFromRow(const T& row) {
        name_ = row.name;
        level_ = row.level;
        xp_ = row.exp;
    }

  template <typename U, typename T>
  void commitToUpdate(T& update) const {
    U chars;
    update.assignments.add(chars.level = level_);
    update.assignments.add(chars.exp = xp_);
  }

    std::string name_;
    uint16_t level_;
    uint32_t xp_;
    uint16_t id_;
    uint32_t tag_;
    int32_t teamId_;
    uint16_t targetId_;
    uint16_t command_;
    mutable std::atomic_bool isOnMap_;
    uint16_t ownerId_;
};
