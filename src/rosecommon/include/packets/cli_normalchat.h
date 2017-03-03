#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon
{
  REGISTER_RECV_PACKET(ePacketType::PAKCS_NORMAL_CHAT, CliNormalChat)

  class CliNormalChat : public CRosePacket {
  public:
    CliNormalChat();
    CliNormalChat(uint8_t buffer[MAX_PACKET_SIZE]);
    CliNormalChat(const std::string& message);

    virtual ~CliNormalChat() = default;

    std::string& message();
    const std::string& message() const;

  protected:
    virtual void pack() override;

  private:
    std::string message_;
  };
}
