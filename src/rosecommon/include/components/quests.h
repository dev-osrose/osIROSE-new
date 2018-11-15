#pragma once

#include <array>

#include "item.h"
#include "iserialize.h"

struct Quest : public RoseCommon::ISerialize {
  static const uint8_t maxSwitches = 32;
  static const uint8_t maxVars = 10;
  static const uint8_t maxItems = 6;

  uint16_t id_{};
  uint32_t timer_{}; // unlimited if 0
  std::array<uint16_t, maxVars> vars_{};
  uint32_t switches_{};
  std::array<RoseCommon::Item, maxItems> items_{};

  virtual bool read(RoseCommon::CRoseReader& reader) override {
      if (!reader.get_uint16_t(id_)) return false;
      if (!reader.get_uint32_t(timer_)) return false;
      for (uint16_t& var : vars_)
          if (!reader.get_uint16_t(var)) return false;
      if (!reader.get_uint32_t(switches_)) return false;
      for (RoseCommon::Item& item : items_)
          if (!reader.get_iserialize(item)) return false;
      return true;
  }

  virtual bool write(RoseCommon::CRoseWriter& writer) const override {
      if (!writer.set_uint16_t(id_)) return false;
      if (!writer.set_uint32_t(timer_)) return false;
      for (const uint16_t var : vars_)
          if (!writer.set_uint16_t(var)) return false;
      if (!writer.set_uint32_t(switches_)) return false;
      for (const RoseCommon::Item& item : items_)
          if (!writer.set_iserialize(item)) return false;
  }

  virtual uint16_t get_size() const override {
      uint16_t size = 0;
      size += sizeof(id_);
      size += sizeof(timer_);
      size += vars_.size() * sizeof(uint16_t);
      size += sizeof(switches_);
      for (const auto& it : items_) size += it.get_size();
      return size;
  }
};

struct Quests : public RoseCommon::ISerialize {
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

  virtual bool read(RoseCommon::CRoseReader& reader) override {
      for (uint16_t& episode : episode_)
          if (!reader.get_uint16_t(episode)) return false;
      for (uint16_t& job : job_)
          if (!reader.get_uint16_t(job)) return false;
      for (uint16_t& planet : planet_)
          if (!reader.get_uint16_t(planet)) return false;
      for (uint16_t& un : union_)
          if (!reader.get_uint16_t(un)) return false;
      for (Quest& quest : quests_)
          if (!reader.get_iserialize(quest)) return false;
      for (uint32_t& switche : switches_)
          if (!reader.get_uint32_t(switche)) return false;
      return true;
  }

  virtual bool write(RoseCommon::CRoseWriter& writer) const override {
      for (const uint16_t episode : episode_)
          if (!writer.set_uint16_t(episode)) return false;
      for (const uint16_t job : job_)
          if (!writer.set_uint16_t(job)) return false;
      for (const uint16_t planet : planet_)
          if (!writer.set_uint16_t(planet)) return false;
      for (const uint16_t un : union_)
          if (!writer.set_uint16_t(un)) return false;
      for (const Quest& quest : quests_)
          if (!writer.set_iserialize(quest)) return false;
      for (const uint32_t switche : switches_)
          if (!writer.set_uint32_t(switche)) return false;
      return true;
  }

  uint16_t get_size() const override {
      uint16_t size = 0;
      size += sizeof(uint16_t) * episode_.size();
      size += sizeof(uint16_t) * job_.size();
      size += sizeof(uint16_t) * planet_.size();
      size += sizeof(uint16_t) * union_.size();
      for (const Quest& it : quests_) size += it.get_size();
      size += sizeof(uint32_t) * switches_.size();
      return size;
  }
};
