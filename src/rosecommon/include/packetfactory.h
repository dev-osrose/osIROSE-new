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
 * \file packetfactory
 * \brief Contains the packet related getPacket & makePacket
 *
 * \author L3nn0x
 * \date october 2016
 */
#pragma once
#include "factory.h"
#include "epackettype.h"
#include "crosepacket.h"
#include "crosewriter.h"

namespace RoseCommon {

using RecvPacketFactory = Core::Factory<ePacketType, std::unique_ptr<CRosePacket>, EPacketTypeHash>;
using SendvPacketFactory = Core::Factory<ePacketType, std::unique_ptr<CRosePacket>, EPacketTypeHash>;

template <typename T>
inline std::unique_ptr<CRosePacket> createPacket_const(const uint8_t *buffer)
{
    return T::allocate(buffer);
}

template <typename T>
inline std::unique_ptr<CRosePacket> createPacket(uint8_t *buffer)
{
    return T::allocate(buffer);
}

/*!
 * \function fetchPacket
 * \brief returns the correctly instantiated packet from a buffer (not compile time).
 * \param[in] the data buffer
 * \param[out] the correct instance of the packet
 *
 * \author L3nn0x
 * \date october 2016
 */
template <bool isServer = false>
inline std::unique_ptr<CRosePacket> fetchPacket(const uint8_t *buffer) {
  if constexpr (isServer) {
     return SendvPacketFactory::create(CRosePacket::type(buffer), const_cast<uint8_t*>(buffer));
  } else {
     return RecvPacketFactory::create(CRosePacket::type(buffer), buffer);
  }
}

void register_recv_packets();
void register_send_packets();

}
