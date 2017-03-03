#include "cli_revivereq.h"

namespace RoseCommon
{
  CliReviveReq::CliReviveReq() : CRosePacket( ePacketType::PAKCS_REVIVE_REQ ) {}

  CliReviveReq::CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket( buffer ) {
    if ( type() != ePacketType::PAKCS_REVIVE_REQ )
      throw std::runtime_error( "Not the right packet!" );
    *this >> type_;
  }

  CliReviveReq::CliReviveReq(CliReviveReq::eType type) : CRosePacket( ePacketType::PAKCS_REVIVE_REQ ),
                                                         type_( type ) {}

  CliReviveReq::eType CliReviveReq::type() const {
    return type_;
  }

  void CliReviveReq::pack() {
    *this << type_;
  }
}
