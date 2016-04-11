
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

		virtual ~CliLoginReq() {}

		std::string password() const {return password_;}
		std::string username() const {return username_;}

	private:
		std::string password_;
		std::string username_;
};

class SrvLoginReply : public CRosePacket {
	public:
		SrvLoginReply(uint8_t result, uint16_t right, uint16_t type) : CRosePacket(ePacketType::PAKLS_LOGIN_REPLY),
		result_(result), right_(right), type_(type) {
			*this << result_ << right_ << type_;
		}

		virtual ~SrvLoginReply() {}

		uint8_t		result() const {return result_;}
		uint16_t	right() const {return right_;}
		uint16_t	type() const {return type_;}

		void	addServer(const std::string &name, uint32_t id, bool isTest = false) {
			*this << (isTest ? '@' : ' ') << name << id;
		}

	private:
		uint8_t result_;
		uint16_t right_;
		uint16_t type_;
};

}

#endif /* !_PACKETCLASSES_H_ */
