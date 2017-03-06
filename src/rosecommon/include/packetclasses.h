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

/*!
 * \file packetclasses.h
 *
 * \author L3nn0x RavenX8
 * \date april 2016
 *
 * This file contains all packet subclasses and the logic needed
 */
#ifndef _PACKETCLASSES_H_
#define _PACKETCLASSES_H_

#include "epackettype.h"
#include "crosepacket.h"
#include <string>
#include <exception>
#include <vector>
#include "iscpackets.pb.h"
#include "packets.h"

//#include "loginpackets.h"
//#include "charpackets.h"
//#include "mappackets.h"

namespace RoseCommon {

//-----------------------------------------------
// ISC Packets
//-----------------------------------------------
REGISTER_RECV_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister)
REGISTER_SEND_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister)
class IscServerRegister : public CRosePacket {
 public:
  IscServerRegister(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::ISC_SERVER_REGISTER)
      throw std::runtime_error("Not the right packet!");
    uint32_t _size = size();
    auto data = std::unique_ptr<uint8_t[]>(new uint8_t[_size]);
    memset(data.get(), 0, _size);
    for (uint32_t i = 0; i < _size; ++i) *this >> data[i];

    if (server_reg_.SerializeToArray(data.get(), _size) == false)
      throw std::runtime_error("Couldn't serialize the data");
  }

  IscServerRegister(const std::string &name, const std::string &ip, int32_t id,
                    int32_t port, int32_t type, int32_t right)
      : CRosePacket(ePacketType::ISC_SERVER_REGISTER) {
    server_reg_.set_name(name);
    server_reg_.set_addr(ip);
    server_reg_.set_port(port);
    server_reg_.set_type((iscPacket::ServerType)type);
    server_reg_.set_accright(right);
    server_reg_.set_id(id);
  }

  iscPacket::ServerReg server_reg() const { return server_reg_; }

 protected:
  void pack() {
    int _size = server_reg_.ByteSize();
    auto data = std::unique_ptr<uint8_t[]>(new uint8_t[_size]);
    memset(data.get(), 0, _size);
    if (server_reg_.SerializeToArray(data.get(), _size) == false)
      throw std::runtime_error("Couldn't serialize the data");

    for (int i = 0; i < _size; ++i) *this << data[i];
  }

 private:
  iscPacket::ServerReg server_reg_;
};

//------------------------------------------------

REGISTER_SEND_PACKET(ePacketType::ISC_SHUTDOWN, IscServerShutdown)
REGISTER_RECV_PACKET(ePacketType::ISC_SHUTDOWN, IscServerShutdown)
class IscServerShutdown : public CRosePacket {
 public:
  IscServerShutdown(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::ISC_SHUTDOWN)
      throw std::runtime_error("Not the right packet!");
    uint32_t _size = size();
    auto data = std::unique_ptr<uint8_t[]>(new uint8_t[_size]);
    memset(data.get(), 0, _size);
    for (uint32_t i = 0; i < _size; ++i) *this >> data[i];

    if (server_shutdown_.SerializeToArray(data.get(), _size) == false)
      throw std::runtime_error("Couldn't serialize the data");
  }

  IscServerShutdown(int32_t id, int32_t type = 0)
      : CRosePacket(ePacketType::ISC_SHUTDOWN) {
    server_shutdown_.set_id(id);
    server_shutdown_.set_type((iscPacket::ServerType)type);
  }

  iscPacket::ServerShutdown server_reg() const { return server_shutdown_; }

 protected:
  void pack() {
    int _size = server_shutdown_.ByteSize();
    auto data = std::unique_ptr<uint8_t[]>(new uint8_t[_size]);
    memset(data.get(), 0, _size);
    if (server_shutdown_.SerializeToArray(data.get(), _size) == false)
      throw std::runtime_error("Couldn't serialize the data");

    for (int i = 0; i < _size; ++i) *this << data[i];
  }

 private:
  iscPacket::ServerShutdown server_shutdown_;
};

//------------------------------------------------
}

#endif /* !_PACKETCLASSES_H_ */
