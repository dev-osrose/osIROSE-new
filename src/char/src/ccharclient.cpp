#include "ccharserver.h"
#include "ccharisc.h"
#include "ccharclient.h"
#include "epackettype.h"
#include "rosepackets.h"
#include "database.h"

using namespace RoseCommon;

CCharClient::CCharClient()
    : CRoseClient(), access_rights_(0), login_state_(eSTATE::DEFAULT) {}

CCharClient::CCharClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT) {}

bool CCharClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return JoinServerReply(getPacket<ePacketType::PAKCS_JOIN_SERVER_REQ>(
          _buffer));  // Allow client to connect
    case ePacketType::PAKCS_CHAR_LIST_REQ:
      return SendCharListReply();
    case ePacketType::PAKCS_CREATE_CHAR_REQ:
      return SendCharCreateReply(
          getPacket<ePacketType::PAKCS_CREATE_CHAR_REQ>(_buffer));
    case ePacketType::PAKCS_DELETE_CHAR_REQ:
      return SendCharDeleteReply(
          getPacket<ePacketType::PAKCS_DELETE_CHAR_REQ>(_buffer));
    case ePacketType::PAKCS_SELECT_CHAR_REQ:
      return SendCharSelectReply(
          getPacket<ePacketType::PAKCS_SELECT_CHAR_REQ>(_buffer));
    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

bool CCharClient::OnReceived() { return CRoseClient::OnReceived(); }

bool CCharClient::JoinServerReply(
    std::unique_ptr<RoseCommon::CliJoinServerReq> P) {
  logger_->trace("JoinServerReply\n");

  uint32_t sessionID = P->session_id();
  std::string password = P->password();

  std::unique_ptr<Core::IResult> res;
  std::string query = fmt::format("CALL GetSession({});", sessionID);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);
  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
      std::string pwd = "";
      res->getString("password", pwd);
      if (pwd == password) {
        res->getInt( "userid", userid_ );

        auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
            SrvJoinServerReply::OK, std::time(nullptr));
        Send(*packet);
      } else {
        auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
            SrvJoinServerReply::INVALID_PASSWORD, std::time(nullptr));
        Send(*packet);
      }
    } else {
      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::FAILED, std::time(nullptr));
      Send(*packet);
    }
  }
  return true;
}

bool CCharClient::SendCharListReply() {
  logger_->trace("CharListReply\n");

  // mysql query to get the characters created.
  std::unique_ptr<Core::IResult> res;
  std::string query = fmt::format(
      "CALL GetCharList({});", userid_);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);

  auto packet = makePacket<ePacketType::PAKCC_CHAR_LIST_REPLY>();
  if (res != nullptr) {
    if (res->size() != 0) {
      std::string _name;
      uint32_t race, level, job, delete_time;
      res->getString("name", _name);
      res->getInt("race", race);
      res->getInt("level", level);
      res->getInt("job", job);
      res->getInt("delete_time", delete_time);

      packet->addCharacter(_name, race, level, job, delete_time);
      packet->addEquipItem(
          0, SrvCharacterListReply::equipped_position::EQUIP_FACE, 1);
      packet->addEquipItem(
          0, SrvCharacterListReply::equipped_position::EQUIP_HAIR, 1);
    }
  }
  Send(*packet);

  return true;
}

bool CCharClient::SendCharCreateReply(
    std::unique_ptr<RoseCommon::CliCreateCharReq> P) {
  logger_->trace("CharCreateReply\n");
  (void)P;
//   std::string query =
//       fmt::format("CALL CreateChar('{}', {}, {}, {}, {}, {}, {});",
//                    Core::CMySQL_Database::escapeData(P->name()), P->race(),
//                    P->stone(), P->hair(), P->face(), P->weapon(), P->zone());
// 
//   Core::IDatabase& database = Core::databasePool.getDatabase();
//   database.QExecute(query);
// 
//   auto packet = makePacket<ePacketType::PAKCC_CREATE_CHAR_REPLY>(
//       0, 0);  // result, isplatinum
//   Send(*packet);

  return true;
}

bool CCharClient::SendCharDeleteReply(
    std::unique_ptr<RoseCommon::CliDeleteCharReq> P) {
  logger_->trace("CharDeleteReply\n");

  if (P->char_id() > 6) return false;

  uint32_t time = 0;
  if (P->isDelete()) {
    // we need to delete the char
  }

  auto packet =
      makePacket<ePacketType::PAKCC_DELETE_CHAR_REPLY>(P->name(), time);
  Send(*packet);
  return true;
}

bool CCharClient::SendCharSelectReply(
    std::unique_ptr<RoseCommon::CliSelectCharReq> P) {
  (void)P;
  logger_->trace("CharSelectReply\n");

  return true;
}
