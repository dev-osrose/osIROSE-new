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
 * \file rosepackets.h
 *
 * \author L3nn0x
 * \date april 2016
 *
 * This file contains two helper functions to find and construct the correct subclass from a ePacketType value
 */
#ifndef _ROSEPACKETS_H_
#define _ROSEPACKETS_H_

#include "crosepacket.h"
#include "findclass.h"
#include <exception>
#include "singleton.h"

namespace RoseCommon {

template <ePacketType T>
std::unique_ptr<decltype(find_recv_class<T>::type)>	getPacket(uint8_t buffer[MAX_PACKET_SIZE]) {
	try {
		return std::unique_ptr<decltype(find_recv_class<T>::type)>(new decltype(find_recv_class<T>::type)(buffer));
	} catch (std::runtime_error &e) {
      (void)e;
		return nullptr;
	}
}

template <ePacketType T, typename... Args>
std::unique_ptr<decltype(find_send_class<T>::type)>	makePacket(Args... args) {
	return std::unique_ptr<decltype(find_send_class<T>::type)>(new decltype(find_send_class<T>::type)(args...));
}

}

#endif /* !_ROSEPACKETS_H_ */
