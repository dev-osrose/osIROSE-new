
#ifndef _PACKETCLASSES_H_
#define _PACKETCLASSES_H_

#include "epackettype.h"
#include "crosepacket.h"
#include <string>
#include <exception>
#include <vector>

namespace RoseCommon {

class CliLoginReq : public CRosePacket {
	public:
		CliLoginReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
			if (type() != ePacketType::PAKCS_LOGIN_REQ)
				throw std::runtime_error("Not the right packet!");
			std::vector<char> password(32);
			std::vector<char> login(16);
			*this >> password >> login;
			password_ = std::string(password.begin(), password.end());
			username_ = std::string(login.begin(), login.end());
		}

		std::string password() const {return password_;}
		std::string username() const {return username_;}

	private:
		std::string password_;
		std::string username_;
};


}

#endif /* !_PACKETCLASSES_H_ */
