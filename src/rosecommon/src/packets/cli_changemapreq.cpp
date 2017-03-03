#include "cli_changemapreq.h"

namespace RoseCommon
{
  CliChangeMapReq::CliChangeMapReq() : CRosePacket( ePacketType::PAKCS_CHANGE_MAP_REQ ) {}

  CliChangeMapReq::CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket( buffer ) {
    if ( type() != ePacketType::PAKCS_CHANGE_MAP_REQ )
      throw std::runtime_error( "Not the right packet!" );
    *this >> weightRate_;
    *this >> z_;
  }

  CliChangeMapReq::CliChangeMapReq(uint8_t weightRate, uint16_t z) : CRosePacket( ePacketType::PAKCS_CHANGE_MAP_REQ ),
                                                                     weightRate_( weightRate ),
                                                                     z_( z ) {}

  uint8_t CliChangeMapReq::weightRate() const {
    return weightRate_;
  }

  uint16_t CliChangeMapReq::z() const {
    return z_;
  }

  void CliChangeMapReq::pack() {
    *this << weightRate_;
    *this << z_;
  }
}
