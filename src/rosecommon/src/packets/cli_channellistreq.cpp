#include "cli_channellistreq.h"

namespace RoseCommon
{
  CliChannelListReq::CliChannelListReq() : CRosePacket( ePacketType::PAKCS_CHANNEL_LIST_REQ ) {}

  CliChannelListReq::CliChannelListReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket( buffer ) {
    if ( type() != ePacketType::PAKCS_CHANNEL_LIST_REQ )
      throw std::runtime_error( "Not the right packet!" );
    *this >> serverId_;
  }

  CliChannelListReq::CliChannelListReq(uint32_t serverId) : CRosePacket( ePacketType::PAKCS_CHANNEL_LIST_REQ ),
                                                            serverId_( serverId ) {}

  uint32_t CliChannelListReq::serverId() const {
    return serverId_;
  }

  void CliChannelListReq::pack() {
    *this << serverId_;
  }
}
