#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::ISC_SERVER_AUTH, IscLoginReq)
REGISTER_RECV_PACKET(ePacketType::ISC_SERVER_AUTH, IscLoginReq)
class IscLoginReq : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t*> init;
	public:
		IscLoginReq();
		IscLoginReq(uint8_t buffer[MAX_PACKET_SIZE]);
		IscLoginReq(const std::string &password, const std::string &username);

		virtual ~IscLoginReq() = default;

		std::string &password();
		const std::string &password() const;
		std::string &username();
		const std::string &username() const;

	protected:
		virtual void pack() override;

	private:
		std::string password_;
		std::string username_;
};

}
