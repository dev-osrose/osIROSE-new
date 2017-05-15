#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <vector>
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKLC_LOGIN_REPLY, SrvLoginReply)
class SrvLoginReply : public CRosePacket {
	public:
        struct ServerInfo {
            std::string name_;
            uint32_t id_;
            bool test_;
        };

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

		SrvLoginReply();
        SrvLoginReply(uint8_t result, uint16_t right, uint16_t type);
		SrvLoginReply(uint8_t result, uint16_t right, uint16_t type, const std::vector<ServerInfo> &servers);

		virtual ~SrvLoginReply() = default;

		uint8_t result() const;
		uint16_t right() const;
        uint16_t &right() { return right_; }
		uint16_t type() const;
		std::vector<ServerInfo> &servers();
		const std::vector<ServerInfo> &servers() const;

        void addServer(const std::string &name, uint32_t id, bool isTest = false);

	protected:
		virtual void pack() override;

	private:
		uint8_t result_;
		uint16_t right_;
		uint16_t type_;
		std::vector<ServerInfo> servers_;
};

}
