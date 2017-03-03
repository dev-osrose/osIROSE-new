#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon
{
  REGISTER_RECV_PACKET(ePacketType::PAKCS_REVIVE_REQ, CliReviveReq)

  class CliReviveReq : public CRosePacket {
  public:
    enum eType : uint8_t {
      REVIVE_POS,
      SAVE_POS,
      START_POS,
      CURRENT_POS
    };

    CliReviveReq();
    CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]);
    CliReviveReq(CliReviveReq::eType type);

    virtual ~CliReviveReq() = default;

    CliReviveReq::eType type() const;

  protected:
    virtual void pack() override;

  private:
    CliReviveReq::eType type_;
  };
}
