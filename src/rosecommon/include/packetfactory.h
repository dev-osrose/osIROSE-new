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

namespace RoseCommon {

using RecvPacketFactory = Core::Factory<ePacketType, std::unique_ptr<CRosePacket>, EPacketTypeHash>;

template <typename T>
inline std::unique_ptr<CRosePacket> createPacket(uint8_t *buffer)
{
  return std::make_unique<T>(buffer);
}

template <ePacketType Type>
struct find_send_class;

template <ePacketType Type>
using find_send_class_t = typename find_send_class<Type>::type;

template <ePacketType Type>
struct find_recv_class;

template <ePacketType Type>
using find_recv_class_t = typename find_recv_class<Type>::type;

#define REGISTER_SEND_PACKET(Type, Class) class Class; template <> struct find_send_class<Type> { typedef Class type; };

#define REGISTER_RECV_PACKET(Type, Class) class Class; template <> struct find_recv_class<Type> { typedef Class type; }; static const RecvPacketFactory::Initializer<uint8_t*> __##Class = RecvPacketFactory::Initializer<uint8_t*>(Type, &createPacket<Class>);

/*!
 * \function fetchPacket
 * \brief returns the correctly instantiated packet from a buffer (not compile time).
 * \param[in] the data buffer
 * \param[out] the correct instance of the packet
 *
 * \author L3nn0x
 * \date october 2016
 */
inline std::unique_ptr<CRosePacket> fetchPacket(uint8_t *buffer) {
  return RecvPacketFactory::create(CRosePacket::type(buffer), buffer);
}

/*!
 * \function getPacket
 * \brief returns the correctly instantiated packet at compile time from a buffer.
 * \param[in] the data buffer
 * \param[out] the correct instance of the packet
 *
 * \author L3nn0x
 * \ate october 2016
 */
template <ePacketType T>
std::unique_ptr<find_recv_class_t<T>> getPacket(uint8_t buffer[MAX_PACKET_SIZE]) {
    return std::make_unique<find_recv_class_t<T>>(buffer);
}

/*!
 * \function makePacket
 * \brief returns a correctly instantiated packet at compile time from the args.
 * \param[in] the arguments needed to create the packet
 * \param[out] the packet ready to be sent
 *
 * \author L3nn0x
 * \date october 2016
 */
template <ePacketType T, typename... Args>
std::unique_ptr<find_send_class_t<T>> makePacket(Args&&... args) {
    return std::make_unique<find_send_class_t<T>>(std::forward<Args>(args)...);
}

}
