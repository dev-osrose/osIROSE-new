#include "cli_screenshottimereq.h"

namespace RoseCommon
{
  CliScreenShotTimeReq::CliScreenShotTimeReq() : CRosePacket( ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ ) {}

  CliScreenShotTimeReq::CliScreenShotTimeReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket( buffer ) {
    if ( type() != ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ )
      throw std::runtime_error( "Not the right packet!" );
    *this >> count_;
  }

  CliScreenShotTimeReq::CliScreenShotTimeReq(uint16_t count) : CRosePacket( ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ ),
                                                               count_( count ) {}

  uint16_t CliScreenShotTimeReq::count() const {
    return count_;
  }

  void CliScreenShotTimeReq::pack() {
    *this << count_;
  }
}
