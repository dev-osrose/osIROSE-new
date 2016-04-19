#include "cmapserver.h"
#include "cmapisc.h"
#include "cmapclient.h"
#include "epackettype.h"

using namespace RoseCommon;

CMapClient::CMapClient() : CRoseClient(), access_rights_(0) {}

CMapClient::CMapClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)), access_rights_(0) {}

bool CMapClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
//    case ePacketType::PAKCS_JOIN_SERVER_REQ:
//      return JoinServerReply(pak);  // Allow client to connect
//    case ePacketType::PAKCS_CHAR_LIST_REQ:
//      return SendCharListReply(pak);
    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

bool CMapClient::OnReceived() { return CRoseClient::OnReceived(); }
