#include "cli_stop.h"

namespace RoseCommon
{
  CliStop::CliStop() : CRosePacket( ePacketType::PAKCS_STOP ) {}

  CliStop::CliStop(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket( buffer ) {
    if ( type() != ePacketType::PAKCS_STOP )
      throw std::runtime_error( "Not the right packet!" );
    *this >> x_;
    *this >> y_;
  }

  CliStop::CliStop(float x, float y) : CRosePacket( ePacketType::PAKCS_STOP ),
                                       x_( x ),
                                       y_( y ) {}

  float CliStop::x() const {
    return x_;
  }

  float CliStop::y() const {
    return y_;
  }

  void CliStop::pack() {
    *this << x_;
    *this << y_;
  }
}
