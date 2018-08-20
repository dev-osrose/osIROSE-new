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

/*
 * epackettype.h
 *
 *  Created on: Nov 10, 2015
 *      Author: ctorres
 */

#ifndef EPACKETTYPE_H_
#define EPACKETTYPE_H_

#include <string>
#include <stdint.h>

namespace RoseCommon {

#ifdef _WIN32
#define PACK(...) __pragma(pack(push, 1)) __VA_ARGS__ __pragma(pack(pop))
#else
#define PACK(...) __VA_ARGS__ __attribute__((__packed__))
#endif

#ifndef MAX_PACKET_SIZE
#define MAX_PACKET_SIZE 0x7FF
#endif

// CS = Client -> server
// SC = server -> server
// SS = server -> server
// LC = Login -> server
// CC = Char -> Client
// WC = World -> client
enum class ePacketType : uint16_t {
  ISCSTART = 0x300,
  ISC_ALIVE,
  ISC_SERVER_AUTH,
  ISC_SERVER_REGISTER,
  ISC_TRANSFER,
  ISC_SHUTDOWN,
  ISCEND,

  // CLIENT PACKETS START HERE!!!
  PAKSTART = 0x700,
  PAKCS_ALIVE = PAKSTART,
  PAKSS_ERROR = PAKCS_ALIVE,
  PAKSS_ANNOUNCE_TEXT,
  PAKSW_ANNOUNCE_CHAT,
  PAKCS_ACCEPT_REQ,
  PAKCS_CHANNEL_LIST_REQ,
  PAKLC_CHANNEL_LIST_REPLY = PAKCS_CHANNEL_LIST_REQ,

  PAKCS_LOGOUT_REQ = 0x707,
  PAKWC_LOGOUT_REPLY = PAKCS_LOGOUT_REQ,
  PAKCS_LOGIN_REQ,
  PAKLC_LOGIN_REPLY = PAKCS_LOGIN_REQ,
  PAKGC_LOGIN_REPLY,
  PAKCS_SRV_SELECT_REQ = 0x70a,
  PAKLC_SRV_SELECT_REPLY = PAKCS_SRV_SELECT_REQ,

  PAKCS_JOIN_SERVER_REQ,
  PAKSC_JOIN_SERVER_REPLY,
  PAKWC_GM_COMMAND,

  PAKWC_GLOBAL_VARS,
  PAKWC_GLOBAL_FLAGS,

  PAKCC_SWITCH_SERVER = 0x711,
  PAKCS_CHAR_LIST_REQ,
  PAKCC_CHAR_LIST_REPLY = PAKCS_CHAR_LIST_REQ,
  PAKCS_CREATE_CHAR_REQ,
  PAKCC_CREATE_CHAR_REPLY = PAKCS_CREATE_CHAR_REQ,  // 0x713
  PAKCS_DELETE_CHAR_REQ,
  PAKCC_DELETE_CHAR_REPLY = PAKCS_DELETE_CHAR_REQ,
  PAKCS_SELECT_CHAR_REQ, // 0x715
  PAKWC_SELECT_CHAR_REPLY = PAKCS_SELECT_CHAR_REQ,

  PAKWC_INVENTORY_DATA,
  PAKWC_SETMONEY,
  PAKWC_SET_ITEM,
  PAKWC_SERVER_DATA,  // 0x719

  PAKWC_QUEST_DATA = 0x71b,
  PAKCS_CHANGE_CHAR_REQ,
  PAKCC_CHAN_CHAR_REPLY = PAKCS_CHANGE_CHAR_REQ,
  PAKWC_SET_MONEY, // 0x71d
  PAKWC_QUEST_REWARD_MONEY,
  PAKWC_QUEST_REWARD_ITEM,
  PAKWC_QUEST_REWARD_ADD_STAT,
  PAKWC_QUEST_REWARD_SET_STAT,
  PAKCS_CANCEL_LOGOUT,
  PAKWC_QUEST_UPDATE,
  PAKWC_WISH_LIST, // 0x724

  PAKCS_QUEST_DATA_REQ = 0x730,
  PAKWC_QUEST_DATA_REPLY = PAKCS_QUEST_DATA_REQ,
  PAKWC_NPC_EVENT,

  PAKWC_GM_COMMAND_CODE = 0x751, // This is for updating client side varibles
  PAKCS_CHANGE_MAP_REQ = 0x753,
  PAKWC_CHANGE_MAP_REPLY = PAKCS_CHANGE_MAP_REQ,
  PAKWC_INIT_DATA,
  PAKCS_REVIVE_REQ,
  PAKWC_REVIVE_REPLY = PAKCS_REVIVE_REQ,
  PAKWC_SET_REVIVE_POS_REPLY,
  PAKCS_SET_SERVER_VAR_REQ, // 0x757
  PAKWC_SET_SERVER_VAR_REPLY = PAKCS_SET_SERVER_VAR_REQ,

  PAKCS_CHAR_INFO_REQ,
  PAKWC_CHAR_INFO_REPLY = PAKCS_CHAR_INFO_REQ,
  PAKCS_SET_WEIGHT_REQ,
  PAKWC_SET_WEIGHT_REPLY = PAKCS_SET_WEIGHT_REQ,

  PAKWC_ADJUST_POSITION = 0x770,
  PAKCS_STOP_MOVING = 0x771, // because it can't anymore
  PAKWC_STOP_MOVING = PAKCS_STOP_MOVING, // This needs to be Impl in the client, client should stop because it can't move anymore

  PAKCS_UPDATE_NPC = 0x774,
  PAKCS_SUMMON_CMD,
  PAKWC_SUMMON_CMD = PAKCS_SUMMON_CMD,

  PAKCS_SET_ANIMATION = 0x781,
  PACWC_SET_ANIMATION = PAKCS_SET_ANIMATION,
  PAKCS_TOGGLE,
  PAKWC_TOGGLE = PAKCS_TOGGLE,
  PAKCS_NORMAL_CHAT,
  PAKWC_NORMAL_CHAT = PAKCS_NORMAL_CHAT,
  PAKCS_WHISPER_CHAT,
  PAKWC_WHISPER_CHAT = PAKCS_WHISPER_CHAT,
  PAKCS_SHOUT_CHAT,
  PAKWC_SHOUT_CHAT = PAKCS_SHOUT_CHAT,
  PAKCS_PARTY_CHAT,
  PAKWC_PARTY_CHAT = PAKCS_PARTY_CHAT,
  PAKCS_CLAN_CHAT,
  PAKWC_CLAN_CHAT = PAKCS_CLAN_CHAT,
  PAKCS_ALLIED_CHAT,
  PAKWC_ALLIED_CHAT = PAKCS_ALLIED_CHAT,
  PAKCS_ALLIED_SHOUT_CHAT,
  PAKWC_ALLIED_SHOUT_CHAT = PAKCS_ALLIED_SHOUT_CHAT,

  PAKWC_EVENT_STATUS = 0x790,
  PAKWC_NPC_CHAR,
  PAKWC_MOB_CHAR,
  PAKWC_PLAYER_CHAR,
  PAKWC_REMOVE_OBJECT,
  PAKCS_SET_POSITION,
  PAKCS_STOP, // client wants to stop
  PAKWC_STOP = PAKCS_STOP, // object stops at position x
  PAKWC_MOVE, // mouse cmd with move mode in it??
  
  PAKCS_ATTACK,
  PAKWC_ATTACK = PAKCS_ATTACK,
  
  PAKCS_DAMAGE,
  PAKWC_DAMAGE = PAKCS_DAMAGE,

  PAKCS_MOUSE_CMD = 0x79A, // client wants to move or click on an object
  PAKWC_MOUSE_CMD = PAKCS_MOUSE_CMD, // answer from the server
  
  PAKWC_SETEXP,
  PAKWC_LEVELUP = 0x79E,
  
  PAKCS_HP_REQ = 0x79F,
  PAKWC_HP_REPLY = PAKCS_HP_REQ,
  
  PAKWC_SET_HP_AND_MP,
  
  PAKCS_STORE_TRADE_REQ,
  PAKWC_STORE_TRADE_REPLY = PAKCS_STORE_TRADE_REQ,

  PAKCS_USE_ITEM = 0x07a3,
  PAKWC_USE_ITEM_REPLY = PAKCS_USE_ITEM,
  
  PAKCS_DROP_ITEM = 0x7A4,

  PAKCS_EQUIP_ITEM = 0x7A5,
  PAKWC_EQUIP_ITEM = PAKCS_EQUIP_ITEM,
  
  PAKWC_DROP_ITEM = 0x07a6,
  
  PAKCS_FIELDITEM_REQ = 0x07a7,
  PAKWC_FIELDITEM_REPLY = PAKCS_FIELDITEM_REQ,
  
  PAKCS_TELEPORT_REQ = 0x07a8,
  PAKWC_TELEPORT_REPLY = PAKCS_TELEPORT_REQ,
  
  PAKCS_SET_HOTBAR_ICON_REQ = 0x07aa,
  PAKWC_SET_HOTBAR_ICON_REPLY = PAKCS_SET_HOTBAR_ICON_REQ,
  
  PAKCS_STORAGE_LIST_REQ = 0x07ad,
  PAKWC_STORAGE_LIST_REPLY = PAKCS_STORAGE_LIST_REQ,
  
  PAKCS_MOVE_ITEM_REQ = 0x07ae,
  PAKWC_MOVE_ITEM_REPLY = PAKCS_MOVE_ITEM_REQ,
  
  PAKWC_LEARN_SKILL = 0x07b0,
  
  PAKCS_LEVEL_SKILL_REQ = 0x07b1,
  PAKWC_LEVEL_SKILL_REPLY = PAKCS_LEVEL_SKILL_REQ,
  
  //TODO: 0x07b2 -> 0x07bd all skill related
  //TODO: 0x07c0 -> 0x07c7 all trading and private store related

  PAKCS_PARTY_REQ = 0x7d0,
  PAKWC_PARTY_REQ = PAKCS_PARTY_REQ,
  PAKCS_PARTY_REPLY,
  PAKWC_PARTY_REPLY = PAKCS_PARTY_REPLY,
  PAKWC_PARTY_MEMBER,
  PAKWC_PARTY_ITEM,
  PAKWC_PARTY_LVLXP,

  PAKCS_PARTY_RULE = 0x7d7,
  PAKWC_PARTY_RULE = PAKCS_PARTY_RULE,

  PAKWC_BILLING_MESSAGE = 0x7de,
  PAKCS_SCREEN_SHOT_TIME_REQ = 0x7eb,
  PAKSC_SCREEN_SHOT_TIME_REPLY = PAKCS_SCREEN_SHOT_TIME_REQ,
  PAKSS_ACCEPT_REPLY = 0x7ff,
  EPACKETMAX,

  STRESS = 0x6F6D
};

inline bool operator!(const ePacketType& rhs) {
  return static_cast<int16_t>(rhs) == 0;
}
inline bool operator!=(const uint32_t& lhs, const ePacketType& rhs) {
  return (lhs != static_cast<uint32_t>(rhs));
}

template <typename E>
constexpr auto to_underlying(E e) noexcept {
  return static_cast<typename std::underlying_type_t<E>>(e);
}

struct EPacketTypeHash {
   template <typename T>
     std::size_t operator()(T t) const noexcept {
     return to_underlying(t);
   }
};


struct tChannelInfo {
  uint16_t ChannelID;
  uint16_t Port;
  uint32_t MinRight;
  std::string channelName;
  std::string IPAddress;

  tChannelInfo()
      : ChannelID(0), Port(0), MinRight(0), channelName(""), IPAddress("") {}
};

}

#endif /* EPACKETTYPE_H_ */
