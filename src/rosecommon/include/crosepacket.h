/*!
 *  \file crosepacket.h
 *
 *  This file contains CRosePacket
 *
 */
#ifndef _CROSEPACKET_H_
#define _CROSEPACKET_H_

#include "epackettype.h"
#include "iscpackets.pb.h"
#include <string>

using namespace iscPacket;

namespace RoseCommon {

class CRosePacket {
	public:
  union {
    struct {
      sPacketHeader Header;
      uint8_t Data[MAX_PACKET_SIZE - 6];
    };
    uint8_t Buffer[MAX_PACKET_SIZE];

    pakEncryptionRequest pEncryptReq;
    pakLoginReply pLoginReply;
    pakChannelList_Req pChannelListReq;
    pakChannel_List pChannelList;
  };

  CRosePacket(unsigned int mycommand, unsigned short mysize = 6,
              unsigned short myunused = 0);

  CRosePacket(ePacketType mycommand, unsigned short mysize = 6,
              unsigned short myunused = 0);

  ~CRosePacket() {}

  void AddString(const std::string &value, bool NullTerminate);
  void AddBytes(const uint8_t *value, uint16_t size);

  void GetString(uint16_t pos, uint16_t size, char* outbuffer);
  void GetBytes(uint16_t pos, uint16_t len, uint8_t* outbuffer);

  // Functions added by Drakia
  template <class T>
  void Add(T value) {
    *((T*)&Buffer[Header.Size]) = value;
    Header.Size += sizeof(T);
  }

  template <class T>
  void AddString(const std::string &value) {
    Add<T>(value.size());
    AddString(value, false);
  }

  // Functions added by Raven
  template <class T>
  T Get(uint16_t pos) {
    return Data[pos];
  }

 private:
  // This is only here until g++ adds c++11 std::strcpy_s
  template <size_t charCount>
  void strcpy_safe(char (&output)[charCount], const char* pSrc) {
    strncpy(output, pSrc, charCount);
    output[charCount - 1] = 0;
  }

  void strcpy_safe(char* output, size_t charCount, const char* pSrc);
};

}

#endif
