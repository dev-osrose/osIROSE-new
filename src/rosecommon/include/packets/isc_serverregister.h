#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include "isccommon.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister)
REGISTER_RECV_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister)
class IscServerRegister : public CRosePacket {
	public:
		IscServerRegister();
		IscServerRegister(uint8_t buffer[MAX_PACKET_SIZE]);
		IscServerRegister(const std::string &name, const std::string &ip, int32_t id, int32_t port, Isc::ServerType type, int32_t right);

		virtual ~IscServerRegister() = default;

    Isc::ServerType serverType() const;
		std::string &name();
		const std::string &name() const;
		std::string &addr();
		const std::string &addr() const;
		int32_t port() const;
		int32_t right() const;
		int32_t id() const;

	protected:
		virtual void pack() override;

	private:
    Isc::ServerType type_;
		std::string name_;
		std::string addr_;
		int32_t port_;
		int32_t right_;
		int32_t id_;
};

}
