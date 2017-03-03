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

#ifndef _LOGINPACKETS_H_
#define _LOGINPACKETS_H_

#include "epackettype.h"
#include "crosepacket.h"
#include "packets.h"
#include <string>
#include <vector>

namespace RoseCommon
{
  //------------------------------------------------

  REGISTER_SEND_PACKET(ePacketType::PAKLC_LOGIN_REPLY, SrvLoginReply)

  class SrvLoginReply : public CRosePacket {
  public:
    SrvLoginReply(uint8_t result, uint16_t right, uint16_t type);

    virtual ~SrvLoginReply();

    uint8_t& result();
    uint16_t& right();
    uint16_t& type();

    void setRight(uint16_t right);
    void addServer(const std::string& name, uint32_t id, bool isTest = false);

    enum eResult : uint8_t {
      OK = 0,
      FAILED,
      UNKNOWN_ACCOUNT,
      INVALID_PASSWORD,
      ALREADY_LOGGEDIN,
      REFUSED_ACCOUNT,
      NEED_CHARGE,
      NO_RIGHT_TO_CONNECT,
      TOO_MANY_USERS,
      NO_NAME,
      INVALID_VERSION,
      OUTSIDE_REGION
    };

  protected:
    void pack();

  private:
    uint16_t right_;
    uint16_t type_;
    uint8_t result_;

    struct info {
      std::string name_;
      uint32_t channel_id_;
      bool test_;

      info(const std::string& name, uint32_t id, bool isTest = false);
    };

    std::vector<info> channel_list_;
  };

  //------------------------------------------------

  REGISTER_SEND_PACKET(ePacketType::PAKLC_CHANNEL_LIST_REPLY, SrvChannelReply)

  class SrvChannelReply : public CRosePacket {
  public:
    SrvChannelReply(uint32_t serverid);

    virtual ~SrvChannelReply();

    uint8_t server_id() const;
    uint16_t channel_count() const;

    void addChannel(const std::string& name, uint8_t id,
                    uint16_t user_capacity_percentage, uint8_t low_age = 0,
                    uint8_t high_age = 0);

  protected:
    void pack();

  private:
    uint32_t server_id_;
    uint8_t channel_count_;

    struct info {
      std::string name_;
      uint8_t channel_id_;
      uint8_t low_age_;
      uint8_t high_age_;
      uint16_t capacity_;

      info(const std::string& name, uint8_t id, uint16_t user_capacity_percentage,
           uint8_t low_age = 0, uint8_t high_age = 0);
    };

    std::vector<info> channel_list_;
  };
}

#endif
