#ifndef __CROSEPACKETS_H__
#define __CROSEPACKETS_H__

#include "crosepacket.h"

class ChannelInfo {
public:
  ChannelInfo(uint8_t channelID, uint16_t percentFilled, const std::string &name)
              : channelID_(channelID), percentFilled_(percentFilled), name_(name) {}

  uint8_t &channelID() { return channelID_; }
  uint8_t &lowAge() { return lowAge_; }
  uint8_t &highAge() { return highAge_; }
  uint16_t &name()

private:
  uint8_t channelID_;
  uint16_t percentFilled_;
  uint8_t lowAge_;
  uint8_t highAge_;
  std::string name_; // null terminated
};

class ChannelInfoList : public CRosePacket {
public:
  ChannelInfoList(const std::vector<ChannelInfo> &data) : CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY) {
    for (const auto &it : data)
      *this << it;
  }

  virtual ~ChannelInfoList() {}

  template <typename T>
  CRosePacket &operator<<(const T &data) {
    return CRosePacket::operator<<<T>(data);
  }
};

template <>
CRosePacket &operator<<<ChannelInfo>(const ChannelInfo &data) {
  *this << data.getChannelID() << data.getLowAge()
        << data.getHighAge() << data.getName() << 0;
  return *this;
}

#endif
