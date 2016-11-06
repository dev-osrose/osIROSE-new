// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "mappackets.h"

namespace RoseCommon {

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::CliLogoutReq::CliLogoutReq()
    : CRosePacket(ePacketType::PAKCS_LOGOUT_REQ) {}

RoseCommon::CliLogoutReq::CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_LOGOUT_REQ)
    throw std::runtime_error("Not the right packet!");
}

RoseCommon::CliLogoutReq::~CliLogoutReq() {}

//--------------------------------------------

RoseCommon::CliMouseCmd::CliMouseCmd() : CRosePacket(ePacketType::PAKCS_MOUSE_CMD) {}

RoseCommon::CliMouseCmd::CliMouseCmd(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_MOUSE_CMD)
        throw std::runtime_error("Not the right packet!");
    *this >> targetId_ >> x_ >> y_ >> z_;
}

RoseCommon::CliMouseCmd::~CliMouseCmd() {}

//---------------------------------------------

RoseCommon::SrvMouseCmd::SrvMouseCmd() : CRosePacket(ePacketType::PAKWC_MOUSE_CMD) {}

RoseCommon::SrvMouseCmd::SrvMouseCmd(uint16_t sourceObjId, uint16_t destObjId,
        uint16_t srvDist, float destX, float destY, int16_t posZ)
    : CRosePacket(ePacketType::PAKWC_MOUSE_CMD),
    sourceId_(sourceObjId), destId_(destObjId), dist_(srvDist),
    x_(destX), y_(destY), z_(posZ) {}

void RoseCommon::SrvMouseCmd::pack() {
    *this << sourceId_ << destId_ << dist_ << x_ << y_ << z_;
}

RoseCommon::SrvMouseCmd::~SrvMouseCmd() {}

//---------------------------------------------

RoseCommon::CliStopMoving::CliStopMoving() : CRosePacket(ePacketType::PAKCS_STOP_MOVING) {}

RoseCommon::CliStopMoving::CliStopMoving(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    *this >> x_ >> y_ >> z_;
}

RoseCommon::CliStopMoving::~CliStopMoving() {}

//---------------------------------------------

RoseCommon::SrvLogoutReply::SrvLogoutReply(uint16_t wait_time)
    : CRosePacket(ePacketType::PAKWC_LOGOUT_REPLY), wait_time_(wait_time) {}

RoseCommon::SrvLogoutReply::~SrvLogoutReply() {}

uint16_t RoseCommon::SrvLogoutReply::wait_time() const { return wait_time_; }

void RoseCommon::SrvLogoutReply::pack() { *this << wait_time_; }
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::CliChangeMapReq::CliChangeMapReq()
    : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ),
      weight_rate_(0),
      position_z_(0) {}

RoseCommon::CliChangeMapReq::CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_CHANGE_MAP_REQ)
    throw std::runtime_error("Not the right packet!");
  *this >> weight_rate_ >> position_z_;
}

RoseCommon::CliChangeMapReq::~CliChangeMapReq() {}

uint8_t RoseCommon::CliChangeMapReq::weight_rate() const {
  return weight_rate_;
}

uint16_t RoseCommon::CliChangeMapReq::position_z() const { return position_z_; }

void RoseCommon::CliChangeMapReq::pack() {
  *this << weight_rate_ << position_z_;
}
//---------------------------------------------
RoseCommon::SrvChangeMapReply::SrvChangeMapReply(
    uint16_t object_index, uint16_t current_hp, uint16_t current_mp,
    uint16_t current_exp, uint16_t penalize_exp, uint16_t world_time,
    uint16_t team_number)
    : CRosePacket(ePacketType::PAKWC_CHANGE_MAP_REPLY),
      object_index_(object_index),
      current_hp_(current_hp),
      current_mp_(current_mp),
      current_exp_(current_exp),
      penalize_exp_(penalize_exp),
      world_time_(world_time),
      team_number_(team_number) {
  for (int idx = 0; idx < MAX_SELL_TYPE; ++idx) {
    zone_vars_.item_rate_[idx] = 0;
  }
}

RoseCommon::SrvChangeMapReply::~SrvChangeMapReply() {}

uint16_t RoseCommon::SrvChangeMapReply::object_index() const {
  return object_index_;
}

uint16_t RoseCommon::SrvChangeMapReply::current_hp() const {
  return current_hp_;
}

uint16_t RoseCommon::SrvChangeMapReply::current_mp() const {
  return current_mp_;
}

void RoseCommon::SrvChangeMapReply::setItemRate(uint8_t type, uint8_t rate) {
  zone_vars_.item_rate_[type] = rate;
}

void RoseCommon::SrvChangeMapReply::pack() {
  *this << object_index_ << current_hp_ << current_mp_ << current_exp_
        << penalize_exp_;
  {  // global_var
    *this << zone_vars_.craft_rate_ << zone_vars_.update_time_
          << zone_vars_.world_rate_ << zone_vars_.town_rate_;
    for (int idx = 0; idx < MAX_SELL_TYPE; ++idx) {
      *this << zone_vars_.item_rate_[idx];
    }
    *this << zone_vars_.flags_;
  }  // global_var
  *this << world_time_ << team_number_;
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::CliChat::CliChat(const std::string &chat /*= ""*/)
    : CRosePacket(ePacketType::PAKCS_NORMAL_CHAT), chat_(chat) {}

RoseCommon::CliChat::CliChat(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_NORMAL_CHAT)
    throw std::runtime_error("Not the right packet!");
  *this >> chat_;
}

std::string RoseCommon::CliChat::getMessage() const { return chat_; }

void RoseCommon::CliChat::pack() { *this << chat_; }
//---------------------------------------------
RoseCommon::CliWhisper::CliWhisper(const std::string &targetId, const std::string &message /*= ""*/)
    : CRosePacket(ePacketType::PAKCS_WHISPER_CHAT), targetId_(targetId), message_(message) {}

RoseCommon::CliWhisper::CliWhisper(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_WHISPER_CHAT)
    throw std::runtime_error("Not the right packet!");
  *this >> targetId_ >> message_;
}

std::string RoseCommon::CliWhisper::targetId() const { return targetId_; }
std::string RoseCommon::CliWhisper::message() const { return message_; }

void RoseCommon::CliWhisper::pack() { *this << targetId_ << message_; }
//---------------------------------------------
//---------------------------------------------
RoseCommon::CliReviveReq::CliReviveReq(uint8_t type /*= 0*/)
    : CRosePacket(ePacketType::PAKCS_REVIVE_REQ), type_(type) {}

RoseCommon::CliReviveReq::CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_REVIVE_REQ)
    throw std::runtime_error("Not the right packet!");
  *this >> type_;
}

RoseCommon::CliReviveReq::eType RoseCommon::CliReviveReq::getType() const {
  return (eType)type_;
}

void RoseCommon::CliReviveReq::pack() { *this << type_; }
//---------------------------------------------
RoseCommon::SrvReviveReply::SrvReviveReply(uint16_t mapid /*= 0*/)
    : CRosePacket(ePacketType::PAKWC_REVIVE_REPLY), mapid_(mapid) {}

void RoseCommon::SrvReviveReply::pack() { *this << mapid_; }

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::SrvInitDataReply::SrvInitDataReply(uint32_t rand_seed,
                                               uint16_t rand_index)
    : CRosePacket(ePacketType::PAKWC_INIT_DATA),
      rand_seed_(rand_seed),
      rand_index_(rand_index) {}

RoseCommon::SrvInitDataReply::~SrvInitDataReply() {}

uint32_t RoseCommon::SrvInitDataReply::rand_seed() const { return rand_seed_; }

uint16_t RoseCommon::SrvInitDataReply::rand_index() const {
  return rand_index_;
}

void RoseCommon::SrvInitDataReply::pack() {
  *this << rand_seed_ << rand_index_;
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::SrvServerData::SrvServerData(uint8_t type)
    : CRosePacket(ePacketType::PAKWC_GLOBAL_VARS), type_(type) {}

RoseCommon::SrvServerData::~SrvServerData() {}

uint8_t RoseCommon::SrvServerData::type() const { return type_; }

void RoseCommon::SrvServerData::pack() {
  *this << type_;

  switch (type_) {
    case data_type::ECONOMY: {
      *this << enconmy_data_.counter_ << enconmy_data_.pop_base_
            << enconmy_data_.dev_base_;
      for (int idx = MIN_SELL_TYPE; idx < MAX_SELL_TYPE; ++idx) {
        *this << enconmy_data_.consume_[idx];
      }
      *this << enconmy_data_.dev_ << enconmy_data_.pop_;
      for (int idx = MIN_SELL_TYPE; idx < MAX_SELL_TYPE; ++idx) {
        *this << enconmy_data_.item_[idx];
      }
      break;
    }
    case data_type::NPC:
    default:
      break;
  }
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

SrvRemoveObject::SrvRemoveObject(Entity entity)
    : CRosePacket(ePacketType::PAKWC_REMOVE_OBJECT), entity_(entity) {}

void SrvRemoveObject::pack() {
    auto basic = entity_.component<BasicInfo>();
    *this << basic->id_;
}

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::SrvSelectCharReply::SrvSelectCharReply(Entity entity) : CRosePacket(ePacketType::PAKWC_SELECT_CHAR_REPLY), entity_(entity) {}

RoseCommon::SrvSelectCharReply::~SrvSelectCharReply() {}

void RoseCommon::SrvSelectCharReply::pack() {
    auto pos = entity_.component<Position>();
    auto basic = entity_.component<BasicInfo>();
    auto stats = entity_.component<Stats>();
    auto advanced = entity_.component<AdvancedInfo>();
    auto graphics = entity_.component<CharacterGraphics>();
    auto info = entity_.component<CharacterInfo>();
    auto skills = entity_.component<Skills>();
    auto hotbar = entity_.component<Hotbar>();
    auto equipped = entity_.component<EquippedItems>();
    auto effects = entity_.component<StatusEffects>();

    *this << graphics->race_ << pos->map_ << pos->x_ << pos->y_ << pos->spawn_;

    for (auto &it : equipped->items_)
        it.partialSerialize(*this);

    *this << info->stone_ << graphics->face_ << graphics->hair_ << info->job_
          << info->factionId_ << info->factionRank_ << info->fame_;

    *this << stats->str_ << stats->dex_ << stats->int_ << stats->con_ << stats->charm_
          << stats->sense_;

    *this << advanced->hp_ << advanced->mp_ << basic->xp_ << basic->level_
          << info->statPoints_ << info->skillPoints_ << stats->bodySize_
          << stats->headSize_ << info->penaltyXp_;
    *this << info->factionFame_;
    *this << info->factionPoints_;
    *this << info->guildId_ << info->guildContribution_ << info->guildRank_ << info->pkFlag_ << info->stamina_;
    for (auto &it : effects->effects_)
        *this << (ISerialize&)it;

    *this << info->patHp_ << info->patCooldownTime_;

    for (auto &it : skills->skills_)
        *this << (ISerialize&)it;

    for (auto &it : hotbar->items_)
        *this << (ISerialize&)it;

    *this << basic->tag_ << basic->name_;
}

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

CliStopReq::CliStopReq( uint8_t buffer[MAX_PACKET_SIZE] )     : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_STOP)
    throw std::runtime_error("Not the right packet!");
  *this >> x >> y;
}
}
