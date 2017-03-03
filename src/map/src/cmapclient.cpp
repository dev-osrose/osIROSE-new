// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cmapserver.h"
#include "cmapisc.h"
#include "cmapclient.h"
#include "epackettype.h"
#include "database.h"
#include "entityComponents.h"
#include <cmath>

using namespace RoseCommon;

CMapClient::CMapClient()
  : CRoseClient(),
    access_rights_( 0 ),
    login_state_( eSTATE::DEFAULT ),
    sessionId_( 0 ),
    userid_( 0 ),
    charid_( 0 ) {}

CMapClient::CMapClient(Core::INetwork* _sock, std::shared_ptr<EntitySystem> entitySystem)
  : CRoseClient( std::move( _sock ) ),
    access_rights_( 0 ),
    login_state_( eSTATE::DEFAULT ),
    sessionId_( 0 ),
    userid_( 0 ),
    charid_( 0 ),
    entitySystem_( entitySystem ) {}

bool CMapClient::HandlePacket(uint8_t* _buffer) {
  switch ( CRosePacket::type( _buffer ) ) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return JoinServerReply( getPacket<ePacketType::PAKCS_JOIN_SERVER_REQ>(
        _buffer ) ); // Allow client to connect
    case ePacketType::PAKCS_CHANGE_MAP_REQ:
      return ChangeMapReply(
        getPacket<ePacketType::PAKCS_CHANGE_MAP_REQ>( _buffer ) );
    default:
      {
        auto packet = fetchPacket( _buffer );
        if ( !packet )
          return CRoseClient::HandlePacket( _buffer );

        if ( login_state_ != eSTATE::LOGGEDIN ) {
          logger_->warn( "Client {} is attempting to execute an action before logging in.",
                         GetId() );
          return true;
        }
        if ( !entitySystem_->dispatch( entity_, *packet ) )
          CRoseClient::HandlePacket( _buffer ); // FIXME : removed the return because I want to be able to mess around with unkown packets for the time being
      }
  }
  return true;
}

CMapClient::~CMapClient() {}

bool CMapClient::OnReceived() { return CRoseClient::OnReceived(); }

void CMapClient::OnDisconnected() {
  entitySystem_->saveCharacter( charid_, entity_ );
  CMapServer::SendPacket( this, CMapServer::eSendType::EVERYONE_BUT_ME, *makePacket<ePacketType::PAKWC_REMOVE_OBJECT>( entity_ ) );
  entitySystem_->destroy( entity_ );
}

bool CMapClient::JoinServerReply(
  std::unique_ptr<RoseCommon::CliJoinServerReq> P) {
  logger_->trace( "CMapClient::JoinServerReply()" );

  if ( login_state_ != eSTATE::DEFAULT ) {
    logger_->warn( "Client {} is attempting to login when already logged in.",
                   GetId() );
    return true;
  }

  uint32_t sessionID = P->sessionId();
  std::string password = Core::CMySQL_Database::escapeData( P->password() );

  std::unique_ptr<Core::IResult> res, itemres;
  std::string query = fmt::format( "CALL get_session({}, '{}');", sessionID, password );

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore( query );
  if ( res != nullptr ) { // Query the DB
    if ( res->size() != 0 ) {
      logger_->debug( "Client {} auth OK.", GetId() );
      login_state_ = eSTATE::LOGGEDIN;
      res->getInt( "userid", userid_ );
      res->getInt( "charid", charid_ );
      sessionId_ = sessionID;
      bool platinium = false;
      res->getInt( "platinium", platinium );
      entity_ = entitySystem_->loadCharacter( charid_, platinium, GetId() );

      if ( entity_ ) {
        entity_.assign<SocketConnector>( this );

        auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::OK, std::time( nullptr ) );
        Send( *packet );

        // SEND PLAYER DATA HERE!!!!!!
        auto packet2 = makePacket<ePacketType::PAKWC_SELECT_CHAR_REPLY>( entity_ );
        Send( *packet2 );

        auto packet3 = makePacket<ePacketType::PAKWC_INVENTORY_DATA>( entity_ );
        Send( *packet3 );

        auto packet4 = makePacket<ePacketType::PAKWC_QUEST_DATA>();
        Send( *packet4 );

        auto packet5 = makePacket<ePacketType::PAKWC_BILLING_MESSAGE>();
        Send( *packet5 );
      }
      else {
        logger_->debug( "Something wrong happened when creating the entity" );
        auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::FAILED, 0 );
        Send( *packet );
      }
    }
    else {
      logger_->debug( "Client {} auth INVALID_PASS.", GetId() );
      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
        SrvJoinServerReply::INVALID_PASSWORD, 0 );
      Send( *packet );
    }
  }
  else {
    logger_->debug( "Client {} auth FAILED.", GetId() );
    auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
      SrvJoinServerReply::FAILED, 0 );
    Send( *packet );
  }
  return true;
};

bool CMapClient::ChangeMapReply(
  std::unique_ptr<RoseCommon::CliChangeMapReq>) {
  logger_->trace( "CMapClient::ChangeMapReply()" );

  if ( login_state_ != eSTATE::LOGGEDIN ) {
    logger_->warn( "Client {} is attempting to change map before logging in.",
                   GetId() );
    return true;
  }
  auto advanced = entity_.component<AdvancedInfo>();
  auto basic = entity_.component<BasicInfo>();
  auto info = entity_.component<CharacterInfo>();

  Send( *makePacket<ePacketType::PAKWC_CHANGE_MAP_REPLY>( GetId(), advanced->hp_, advanced->mp_, basic->xp_, info->penaltyXp_, std::time( nullptr ), 0 ) );
  CMapServer::SendPacket( this, CMapServer::eSendType::EVERYONE_BUT_ME,
                          *makePacket<ePacketType::PAKWC_PLAYER_CHAR>( entity_ ) );

  entitySystem_->processEntities<CharacterGraphics, BasicInfo>( [entity_ = entity_, this](Entity entity) {
      auto basic = entity.component<BasicInfo>();
      if ( entity != entity_ && basic->loggedIn_ )
        this->Send( *makePacket<ePacketType::PAKWC_PLAYER_CHAR>( entity ) );
      return true;
    } );

  basic->loggedIn_ = true;

  return true;
}

bool CMapClient::IsNearby(const CRoseClient* _otherClient) const {
  logger_->trace( "CMapClient::IsNearby()" );
  return EntitySystem::isNearby( entity_, _otherClient->getEntity() );
}
