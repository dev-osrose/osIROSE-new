#include "cli_partyreq.h"

namespace RoseCommon
{
  CliPartyReq::CliPartyReq() : CRosePacket( ePacketType::PAKCS_PARTY_REQ ) {}

  CliPartyReq::CliPartyReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket( buffer ) {
    if ( type() != ePacketType::PAKCS_PARTY_REQ )
      throw std::runtime_error( "Not the right packet!" );
    *this >> request_;
    *this >> idXorTag_;
  }

  CliPartyReq::CliPartyReq(CliPartyReq::Request request, uint32_t idXorTag) : CRosePacket( ePacketType::PAKCS_PARTY_REQ ),
                                                                              request_( request ),
                                                                              idXorTag_( idXorTag ) {}

  CliPartyReq::Request CliPartyReq::request() const {
    return request_;
  }

  uint32_t CliPartyReq::idXorTag() const {
    return idXorTag_;
  }

  void CliPartyReq::pack() {
    *this << to_underlying( request_ );
    *this << idXorTag_;
  }
}
