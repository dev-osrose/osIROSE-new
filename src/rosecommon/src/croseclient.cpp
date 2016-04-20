#include <ctime>
#include "croseclient.h"
#include "epackettype.h"
#include "rosepackets.h"


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

bool CRoseClient::Send(CRosePacket &_buffer) {
  return CRoseClient::Send(_buffer.getPacked());
}

bool CRoseClient::Send(std::unique_ptr<uint8_t[]> _buffer) {
//#ifdef SPDLOG_TRACE_ON
  logger_->trace("Sending a packet on CRoseClient: Header[{0}, 0x{1:x}]", CRosePacket::size(_buffer.get()), (uint16_t)CRosePacket::type(_buffer.get()));
  fmt::MemoryWriter out;
  for(int i = 0; i < CRosePacket::size(_buffer.get()); i++)
    out.write("0x{0:x} ", _buffer[i]);
  logger_->trace("{}", out.c_str());
//#endif
  return CNetwork_Asio::Send(std::move(_buffer));
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
#ifndef DISABLE_CRYPT
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
#ifndef DISABLE_CRYPT
  if (!crypt_.decodeClientBody((unsigned char*)&buffer_)) {
    // ERROR!!!
    logger_->debug() << "Client sent illegal block";
    ResetBuffer();
    return false;
  }
#endif

  logger_->trace("Received a packet on CRoseClient: Header[{0}, 0x{1:x}]", CRosePacket::size(buffer_), (uint16_t)CRosePacket::type(buffer_));
//#ifdef SPDLOG_TRACE_ON
  fmt::MemoryWriter out;
  for(int i = 0; i < CRosePacket::size(buffer_); i++)
    out.write("0x{0:x} ", buffer_[i]);
  logger_->trace("{}", out.c_str());
//#endif
  rtnVal = HandlePacket(buffer_);
  ResetBuffer();

  return rtnVal;
}

bool CRoseClient::OnSend(uint8_t* _buffer) {
  (void)_buffer;
#ifndef DISABLE_CRYPT
  crypt_.encodeServerPacket(_buffer);
#endif
  return true;
}
void CRoseClient::OnSent() {}

bool CRoseClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_ALIVE: {
#ifdef STRESS_TEST
//      CRosePacket* pak =
//          new CRosePacket(ePacketType::PAKCS_ALIVE, sizeof(sPacketHeader));
//      Send(pak);
#endif
      return CNetwork_Asio::HandlePacket(_buffer);
    }
#ifdef STRESS_TEST
//    case (ePacketType)0x6F6D: {
//      CRosePacket* pak = new CRosePacket(0x6F6D, 8);
//      memcpy(pak->Buffer, _buffer, 8);
//      Send(pak);
//      break;
//    }
#endif
    case ePacketType::PAKCS_ACCEPT_REQ: {
      // Encryption stuff
      auto packet = makePacket<ePacketType::PAKSS_ACCEPT_REPLY>(std::time(nullptr));
      Send(*packet);
      break;
    }
    default: {
      logger_->warn("Unknown Packet Type: 0x{0:x}", (uint16_t)CRosePacket::type(_buffer));
      return false;
    }
  }
  return true;
}

}
