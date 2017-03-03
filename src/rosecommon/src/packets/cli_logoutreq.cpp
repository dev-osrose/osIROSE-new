#include "cli_logoutreq.h"

namespace RoseCommon
{
  CliLogoutReq::CliLogoutReq() : CRosePacket( ePacketType::PAKCS_LOGOUT_REQ ) {}

  CliLogoutReq::CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket( buffer ) {
    if ( type() != ePacketType::PAKCS_LOGOUT_REQ )
      throw std::runtime_error( "Not the right packet!" );
  }

  void CliLogoutReq::pack() {}
}
