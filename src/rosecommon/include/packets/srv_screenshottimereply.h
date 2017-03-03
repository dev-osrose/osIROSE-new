#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon
{
  REGISTER_SEND_PACKET(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY, SrvScreenShotTimeReply)

  class SrvScreenShotTimeReply : public CRosePacket {
  public:
    SrvScreenShotTimeReply();
    SrvScreenShotTimeReply(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min);

    virtual ~SrvScreenShotTimeReply() = default;

    uint16_t year() const;
    uint8_t month() const;
    uint8_t day() const;
    uint8_t hour() const;
    uint8_t min() const;

  protected:
    virtual void pack() override;

  private:
    uint16_t year_;
    uint8_t month_;
    uint8_t day_;
    uint8_t hour_;
    uint8_t min_;
  };
}
