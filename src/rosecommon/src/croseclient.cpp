#include <ctime>
#include "croseclient.h"
#include "epackettype.h"


namespace RoseCommon {
//#define STRESS_TEST

CRoseClient::CRoseClient() : CNetwork_Asio(), crypt_() {
  ResetBuffer();
}

CRoseClient::CRoseClient(tcp::socket &&_sock) : CNetwork_Asio(), crypt_() {
  SetSocket(std::move(_sock));
  Recv();
  ResetBuffer();
}

CRoseClient::~CRoseClient() { Shutdown(); }

bool CRoseClient::Send(const CRosePacket &_buffer) {
  /* return CNetwork_Asio::Send(_buffer.createBuffer()); */
  (void)_buffer;
  return true;
}

bool CRoseClient::Send(std::unique_ptr<uint8_t[]> _buffer) {
  (void)_buffer;
  return true;
}

// Callback functions
bool CRoseClient::OnConnect() { return true; }

void CRoseClient::OnConnected() { CNetwork_Asio::OnConnected(); }

bool CRoseClient::OnDisconnect() { return true; }

void CRoseClient::OnDisconnected() {}

bool CRoseClient::OnReceive() { return true; }

bool CRoseClient::OnReceived() {
  bool rtnVal = true;
  if (packet_size_ == 6) {
#ifndef STRESS_TEST
    packet_size_ = crypt_.decodeClientHeader((unsigned char*)&buffer_);
#else
    packet_size_ = buffer_[0];
#endif

    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      logger_->debug() << "Client sent incorrect block header";
      ResetBuffer();
      return false;
    }

    if (packet_size_ > 6) return true;
  }

// decrypt packet now
#ifndef STRESS_TEST
  if (!crypt_.decodeClientBody((unsigned char*)&buffer_)) {
    // ERROR!!!
    logger_->debug() << "Client sent illegal block";
    ResetBuffer();
    return false;
  }
#endif

  /* CRosePacket* pak = (CRosePacket*)&buffer_; */
  /* logger_->debug("Received a packet on CRoseClient: Header[{0}, 0x{1:x}]", pak->Header.Size, (uint16_t)pak->Header.Command); */
  rtnVal = HandlePacket(buffer_);
  ResetBuffer();

  return rtnVal;
}

bool CRoseClient::OnSend(uint8_t* _buffer) {
  (void)_buffer;
#ifndef STRESS_TEST
  crypt_.encodeServerPacket(_buffer);
#endif
  return true;
}
void CRoseClient::OnSent() {}

bool CRoseClient::HandlePacket(uint8_t* _buffer) {
	(void)_buffer;
  /* CRosePacket* pak = (CRosePacket*)_buffer; */
  /* switch ((ePacketType)pak->Header.Command) { */
    /* case ePacketType::PAKCS_ALIVE: { */
//      log_.icprintf("Got keep alive packet\n");
#ifdef STRESS_TEST
      /* CRosePacket* pak = */
      /*     new CRosePacket(ePacketType::PAKCS_ALIVE, sizeof(sPacketHeader)); */
      /* Send(pak); */
#endif
      /* break; */
    /* } */
#ifdef STRESS_TEST
    /* case (ePacketType)0x6F6D: { */
      /* CRosePacket* pak = new CRosePacket(0x6F6D, 8); */
      /* memcpy(pak->Buffer, _buffer, 8); */
      /* Send(pak); */
      /* break; */
    /* } */
#endif
    /* case ePacketType::PAKCS_ACCEPT_REQ: { */
    /*   // Encryption stuff */
      /* CRosePacket* pak = new CRosePacket(0x7ff, sizeof(pakEncryptionRequest)); */
      /* pak->pEncryptReq.Unknown = 0x02; */
      /* pak->pEncryptReq.RandValue = static_cast<uint32_t>(std::time(nullptr)); */
      /* Send(pak); */
      /* break; */
    /* } */
    /* default: { */
    /*   logger_->warn("Unknown Packet Type: 0x{0:x}", (uint16_t)pak->Header.Command); */
      /* return false; */
    /* } */
  /* } */
  return true;
}

}
