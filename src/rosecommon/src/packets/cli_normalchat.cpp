#include "cli_normalchat.h"

namespace RoseCommon
{
  CliNormalChat::CliNormalChat() : CRosePacket( ePacketType::PAKCS_NORMAL_CHAT ) {}

  CliNormalChat::CliNormalChat(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket( buffer ) {
    if ( type() != ePacketType::PAKCS_NORMAL_CHAT )
      throw std::runtime_error( "Not the right packet!" );
    *this >> message_;
  }

  CliNormalChat::CliNormalChat(const std::string& message) : CRosePacket( ePacketType::PAKCS_NORMAL_CHAT ),
                                                             message_( message ) {}

  std::string& CliNormalChat::message() {
    return message_;
  }

  const std::string& CliNormalChat::message() const {
    return message_;
  }

  void CliNormalChat::pack() {
    *this << message_;
  }
}
