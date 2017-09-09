#pragma once

#include <array>

#include "item.h"

struct Quest {
  static const maxSwitches = 32;
  static const maxVars = 10;
  static const maxItems = 6;

  uint16_t id_;
  uint32_t timer_; // unlimited if 0
  std::array<int16_t, maxVars> vars_;
  std::array<uint8_t, maxSwitches / 8> switches_;
  std::array<RoseCommon::Item, maxItems> items_;
};

struct Quests {
  static const uint8_t maxQuests = 10;
  static const uint8_t maxConditionsEpisode = 5;
  static const uint8_t maxConditionsJob = 3;
  static const uint8_t maxConditionsPlanet = 7;
  static const uint8_t maxConditionsUnion = 10;
  static const uint8_t maxSwitches = 16;

  std::array<uint16_t, maxConditionsEpisode> episode_;
  std::array<uint16_t, maxConditionsJob> job_;
  std::array<uint16_t, maxConditionsPlanet> planet_;
  std::array<uint16_t, maxConditionsUnion> union_;
  std::array<Quest, maxQuests> quests_;
  std::array<uint32_t, maxSwitches> switches_;
};
