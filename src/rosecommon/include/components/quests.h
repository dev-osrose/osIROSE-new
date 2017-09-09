#pragma once

#include <array>

#include "item.h"

struct Quest {
  static const uint8_t maxSwitches = 32;
  static const uint8_t maxVars = 10;
  static const uint8_t maxItems = 6;

  uint16_t id_{};
  uint32_t timer_{}; // unlimited if 0
  std::array<uint16_t, maxVars> vars_{};
  uint32_t switches_{};
  std::array<RoseCommon::Item, maxItems> items_{};

  friend
  RoseCommon::CRosePacket &operator<<(RoseCommon::CRosePacket& os, const Quest& q) {
    os << q.id_ << q.timer_;
    os << q.vars_;
    os << q.switches_;
    for (const auto &it : q.items_)
      os << it.getHeader() << it.getData();
    return os;
  }
};

struct Quests {
  static const uint8_t maxQuests = 10;
  static const uint8_t maxConditionsEpisode = 5;
  static const uint8_t maxConditionsJob = 3;
  static const uint8_t maxConditionsPlanet = 7;
  static const uint8_t maxConditionsUnion = 10;
  static const uint8_t maxSwitches = 16;

  std::array<uint16_t, maxConditionsEpisode> episode_{};
  std::array<uint16_t, maxConditionsJob> job_{};
  std::array<uint16_t, maxConditionsPlanet> planet_{};
  std::array<uint16_t, maxConditionsUnion> union_{};
  std::array<Quest, maxQuests> quests_{};
  std::array<uint32_t, maxSwitches> switches_{};

  friend
  RoseCommon::CRosePacket &operator<<(RoseCommon::CRosePacket& os, const Quests& q) {
    os << q.episode_;
    os << q.job_;
    os << q.planet_;
    os << q.union_;
    os << q.quests_;
    os << q.switches_;
    return os;
  }
};
