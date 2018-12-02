#pragma once

struct AdvancedInfo {
    enum MoveMode {
        WALK = 0,
        RUN = 1,
        DRIVE = 2,
        RIDEON = 4
    };

    AdvancedInfo() : zuly_(0), hp_(1), mp_(0), moveMode_(AdvancedInfo::RUN), runSpeed_(0), atkSpeed_(0), weightRate_(0) {}
    template <typename T>
    AdvancedInfo(const T& row) : AdvancedInfo() {
        loadFromRow(row);
    }

    template <typename T>
    void loadFromRow(const T& row) {
        zuly_ = row.zuly;
        hp_ = row.currentHp;
        mp_ = row.currentMp;
    }

  template <typename U, typename T>
  void commitToUpdate(T& update) const {
    U chars;
    update.assignments.add(chars.zuly = zuly_);
    update.assignments.add(chars.currentHp = hp_);
    update.assignments.add(chars.currentMp = mp_);
  }

    int64_t zuly_;
    int32_t hp_;
    int32_t mp_;
    uint8_t moveMode_;
    uint16_t runSpeed_;
    uint16_t atkSpeed_;
    uint8_t weightRate_;
};
