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
  PAKLC_SRV_SELECT_REQ = PAKCS_SRV_SELECT_REQ,

  PAKCS_JOIN_SERVER_REQ,
  PAKSC_JOIN_SERVER_REPLY,

  PAKCC_SWITCH_SERVER = 0x711,
  PAKCS_CHAR_LIST_REQ,
  PAKCC_CHAR_LIST_REPLY = PAKCS_CHAR_LIST_REQ,
  PAKCS_CREATE_CHAR_REQ,
  PAKCC_CREATE_CHAR_REPLY = PAKCS_CREATE_CHAR_REQ,  // 0x713
  PAKCS_DELETE_CHAR_REQ,
  PAKCC_DELETE_CHAR_REPLY = PAKCS_DELETE_CHAR_REQ,
  PAKCS_SELECT_CHAR_REQ,
  PAKWC_SELECT_CHAR_REPLY = PAKCS_SELECT_CHAR_REQ,

  PAKWC_INVENTORY_DATA,
  PAKWC_SETMONEY,
  PAKWC_SET_ITEM,
  PAKWC_SERVER_DATA,  // 0x719

  PAKWC_QUEST_DATA = 0x71b,
  PAKCS_CHANGE_CHAR_REQ,
  PAKCC_CHAN_CHAR_REPLY = PAKCS_CHANGE_CHAR_REQ,
  PAKWC_SET_MONEY,
  PAKWC_QUEST_REWARD_MONEY,
  PAKWC_QUEST_REWARD_ITEM,
  PAKWC_QUEST_REWARD_ADD_STAT,
  PAKWC_QUEST_REWARD_SET_STAT,
  PAKCS_CANCEL_LOGOUT,
  PAKWC_QUEST_UPDATE,

  PAKCS_QUEST_DATA_REQ = 0x730,
  PAKWC_QUEST_DATA_REPLY = PAKCS_QUEST_DATA_REQ,

  PAKCS_CHANGE_MAP_REQ = 0x753,
  PAKWC_CHANGE_MAP_REPLY = PAKCS_CHANGE_MAP_REQ,

  EPACKETMAX,

  STRESS = 0x6F6D
};

inline bool operator!(const ePacketType& rhs) {
  return static_cast<int16_t>(rhs) == 0;
}
inline bool operator!=(const uint32_t& lhs, const ePacketType& rhs) {
  return (lhs != static_cast<uint32_t>(rhs));
}

struct tChannelInfo {
  uint16_t ChannelID;
  uint16_t Port;
  uint32_t MinRight;
  std::string channelName;
  std::string IPAddress;

  tChannelInfo()
      : ChannelID(0), Port(0), MinRight(0), channelName(""), IPAddress("") {}
};

//-------------------------------------------
// GAME PACKETS!!!!!!
//-------------------------------------------
PACK(
    // Packet information
struct sPacketHeader {
  uint16_t Size;        // Packet size
  ePacketType Command;  // Packet command
  uint16_t Unused;      // unused?
});

PACK(
struct pakChannelInfo {
  uint8_t ChannelID;
  uint16_t
      pad;  // This was used to tell the client the AGE of the server. 2 bytes.
  uint16_t Status;  // This is actually the fill percentage of the server.
                    // uint32_t Right;
                    // Channel Name as string
});

PACK(
struct pakChannel_List : public sPacketHeader {
  uint32_t lServerID;
  uint8_t bServerCount;
  // channelInfo sServers[]; // There is a better way to do this, just can't
  // think of one ATM. -Raven
});

PACK(
struct pakChannelList_Req : public sPacketHeader {
  uint32_t lServerID;
});

PACK(
struct pakLoginReply : public sPacketHeader {
  uint8_t Result;
  uint16_t Right;
  uint16_t Type;
});

PACK(
struct pakEncryptionRequest : public sPacketHeader {
  uint8_t Unknown;
  uint32_t RandValue;
});

struct basicCharInfo {
  uint8_t  race;  // This should be moved to the end of the struct for byte alignment reasons
  uint16_t level;
  uint16_t job;
  uint32_t time_until_delete;
};

PACK(struct pakCharList
     : public sPacketHeader {
  uint8_t count;
//  for( i < count ){
//    basicCharinfo
//    equipItem [max_body_items]
//    char name[]
//  }
});

}

#endif /* EPACKETTYPE_H_ */
