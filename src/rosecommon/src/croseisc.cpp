#include "croseisc.h"
#include "epackettype.h"

namespace RoseCommon {

CRoseISC::CRoseISC() : CRoseClient() {
  log_.SetIdentity("CRoseISC");
  ResetBuffer();
}

CRoseISC::CRoseISC(tcp::socket _sock) : CRoseClient(std::move(_sock)) {
  log_.SetIdentity("CRoseISC");
  ResetBuffer();
}

CRoseISC::~CRoseISC() {}

void CRoseISC::OnConnected() {
  // Do encryption handshake here
  CRoseClient::OnConnected();
}

bool CRoseISC::OnDisconnect() { return true; }

void CRoseISC::OnDisconnected() {}

bool CRoseISC::OnReceived() {
  bool rtnVal = true;
  // m_Log.oicprintf( CL_WHITE "Size: %i\n", PacketSize );
  if (packet_size_ == 6) {
    packet_size_ = (uint16_t)buffer_[0];
    // m_Log.oicprintf( CL_WHITE "Size From buffer: %i\n", PacketSize );
    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      log_.eicprintf("Client sent incorrect blockheader\n");
      ResetBuffer();
      return false;
    }

    if (packet_size_ > 6) return true;
  }

  CRosePacket* pak = (CRosePacket*)&buffer_;
  log_.oicprintf("Received a packet on CRoseISC: Header[%i, 0x%X]\n",
                 pak->Header.Size, pak->Header.Command);
  rtnVal = HandlePacket(buffer_);
  ResetBuffer();
  return rtnVal;
}

bool CRoseISC::OnSend(uint8_t* _buffer) {
  // TODO: Encrypt the buffer here!
  (void)_buffer;
  return true;
}

void CRoseISC::OnSent() {}

bool CRoseISC::HandlePacket(uint8_t* _buffer) {
  CRosePacket* pak = (CRosePacket*)_buffer;
  switch (pak->Header.Command) {
    case ePacketType::ISC_ALIVE:
      return true;
    default: {
      log_.eicprintf("Unknown Packet Type: 0x%X\n", pak->Header.Command);
      return false;
    }
  }
  return true;
}
}
