#ifndef _PACKETCLASSES_H_
#define _PACKETCLASSES_H_

#include "epackettype.h"
#include "crosepacket.h"
#include <string>
#include <exception>
#include <vector>

namespace RoseCommon {

//---------------------------------------------------
// Server Recv packets
//---------------------------------------------------
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
		CliLoginReq(const std::string &user, const std::string &pass) : CRosePacket(ePacketType::PAKCS_LOGIN_REQ),
			password_(pass), username_(user) {}

		virtual ~CliLoginReq() {}

		std::string password() const {return password_;}
		std::string username() const {return username_;}

	protected:
                void pack() {
                        *this << password_ << username_;
                }

	private:
		std::string password_;
		std::string username_;
};

class CliChannelReq : public CRosePacket
{
public:
  CliChannelReq( uint8_t buffer[MAX_PACKET_SIZE] ) : CRosePacket( buffer )
  {
    if (type() != ePacketType::PAKCS_CHANNEL_LIST_REQ)
      throw std::runtime_error( "Not the right packet!" );
    *this >> server_id_;
  }

  virtual ~CliChannelReq()
  {
  }

  uint32_t server_id() const
  {
    return server_id_;
  }

private:
  uint32_t server_id_;
};

//-----------------------------------------------
// Send Packets
//-----------------------------------------------

class SrvLoginReply : public CRosePacket {
	public:
		SrvLoginReply(uint8_t result, uint16_t right, uint16_t type) : CRosePacket(ePacketType::PAKLC_LOGIN_REPLY),
		result_(result), right_(right), type_(type) {}

		virtual ~SrvLoginReply() {}

		uint8_t		&result() {return result_;}
		uint16_t	&right() {return right_;}
		uint16_t	&type() {return type_;}

		void	addServer(const std::string &name, uint32_t id, bool isTest = false) {
			*this << (isTest ? '@' : ' ') << name << id;
		}

	protected:
		void pack() {
			*this << result_ << right_ << type_;
		}

	private:
		uint8_t result_;
		uint16_t right_;
		uint16_t type_;
};

class SrvChannelReply : public CRosePacket {
	public:
		SrvChannelReply(uint32_t serverid, uint8_t count) : CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY),
		server_id_(serverid), channel_count_(count) {
			*this << server_id_ << channel_count_;
		}

		virtual ~SrvChannelReply() {}

		uint8_t		server_id() const {return server_id_;}
		uint16_t	channel_count() const {return channel_count_;}

		void	addChannel(const std::string &name, uint8_t id, uint16_t user_capacity_percentage, uint8_t low_age = 0, uint8_t high_age = 0) {
			*this << id << low_age << high_age << user_capacity_percentage << name;
		}

	private:
		uint32_t server_id_;
		uint8_t channel_count_;
};

}

#endif /* !_PACKETCLASSES_H_ */
