#include "ccharserver.h"
#include "ccharisc.h"
#include "ccharclient.h"
#include "epackettype.h"

using namespace RoseCommon;

CCharClient::CCharClient() : CRoseClient(), access_rights_(0) {
  log_.SetIdentity("CCharClient");
}

CCharClient::CCharClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)), access_rights_(0) {
  log_.SetIdentity("CCharClient");
}

bool CCharClient::HandlePacket(uint8_t* _buffer) {
  CRosePacket* pak = (CRosePacket*)_buffer;
  switch (pak->Header.Command) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return JoinServerReply(pak);  // Allow client to connect
    case ePacketType::PAKCS_CHAR_LIST_REQ:
      return SendCharListReply(pak);  // SendCharList( pak );
    case ePacketType::PAKCS_CREATE_CHAR_REQ:
      return SendCharCreateReply(pak);
    case ePacketType::PAKCS_DELETE_CHAR_REQ:
      return SendCharDeleteReply(pak);
    case ePacketType::PAKCS_SELECT_CHAR_REQ:
      return SendCharSelectReply(pak);
    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

bool CCharClient::OnReceived() { return CRoseClient::OnReceived(); }

bool CCharClient::JoinServerReply(CRosePacket* P) {
  log_.icprintf("JoinServerReply\n");

  SetId(P->Get<uint32_t>(0));
  uint8_t _pass[33];
  P->GetBytes(4, 32, _pass);
  _pass[32] = 0;

  return true;
}

bool CCharClient::SendCharListReply(CRosePacket* P) {
  (void)P;
  log_.icprintf("CharListReply\n");

  return true;
}

bool CCharClient::SendCharCreateReply(CRosePacket* P) {
  (void)P;
  log_.icprintf("CharCreateReply\n");

  return true;
}

bool CCharClient::SendCharDeleteReply(CRosePacket* P) {
  log_.icprintf("CharDeleteReply\n");
  uint8_t charid = P->Get<uint8_t>( 0 );
  bool _delete = P->Get<uint8_t>( 1 );

  if( charid > 6 )
    return false;

  char name[17];
  memset(name, 0, 17);
  P->GetString(0, 16, name);
  if(_delete)
  {
    // we need to delete the char
  }

  return true;
}

bool CCharClient::SendCharSelectReply(CRosePacket* P) {
  (void)P;
  log_.icprintf("CharSelectReply\n");

  return true;
}
