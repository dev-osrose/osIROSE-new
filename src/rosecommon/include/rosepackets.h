
#ifndef _ROSEPACKETS_H_
#define _ROSEPACKETS_H_

#include "crosepacket.h"
#include "findclass.h"
#include <exception>

namespace RoseCommon {

template <ePacketType T>
std::unique_ptr<find_recv_class<T>::type>	getPacket(uint8_t buffer[MAX_PACKET_SIZE]) {
	try {
		return new find_recv_class<T>::type(buffer);
	} catch (std::runtime_error &e) {
		return nullptr;
	}
}

template <ePacketType T, typename... Args>
std::unique_ptr<find_send_class<T>::type>	makePacket(Args... args) {
	return new find_send_class<T>::type(args...);
}

}

#endif /* !_ROSEPACKETS_H_ */
