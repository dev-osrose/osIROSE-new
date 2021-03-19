// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __CMAPCLIENT_H__
#define __CMAPCLIENT_H__

#include "croseclient.h"
#include "crosepacket.h"
#include "entity_system.h"

#include "srv_player_char.h"
#include "cli_join_server_req.h"
#include "cli_change_char_req.h"
#include "srv_npc_char.h"
#include "srv_mob_char.h"
#include "srv_drop_item.h"

#include <memory>

class CMapClient : public RoseCommon::CRoseClient, public std::enable_shared_from_this<CMapClient> {
 public:
  CMapClient();
  CMapClient(std::unique_ptr<Core::INetwork> _sock, std::shared_ptr<EntitySystem>);

  virtual ~CMapClient();

  uint32_t get_session_id() const { return sessionId_; }
 
  void send(const RoseCommon::CRosePacket& packet, bool force = false);

  void set_on_map();
  bool is_on_map() const { return login_state_ == eSTATE::ONMAP; }
  void switch_server();
 
  static RoseCommon::Packet::SrvPlayerChar create_srv_player_char(const EntitySystem&, Entity);
  static RoseCommon::Packet::SrvNpcChar create_srv_npc_char(const EntitySystem&, Entity);
  static RoseCommon::Packet::SrvMobChar create_srv_mob_char(const EntitySystem&, Entity entity);
  static RoseCommon::Packet::SrvDropItem create_srv_drop_item(const EntitySystem&, Entity entity);

 protected:
  virtual bool handlePacket(uint8_t* _buffer) override;
  virtual void onDisconnected() override;

  void updateSession();

  bool logoutReply();
  bool joinServerReply(RoseCommon::Packet::CliJoinServerReq&& P);
  bool changeCharacterReply(RoseCommon::Packet::CliChangeCharReq&& P);

  enum class eSTATE {
    DEFAULT,
    LOGGEDIN,
    ONMAP,
    SWITCHING,
  };

  uint16_t access_rights_;
  eSTATE login_state_;
  uint32_t sessionId_;
  uint32_t userid_;
  uint32_t charid_;

  std::shared_ptr<EntitySystem> entitySystem;
  Entity entity;
};

#endif
