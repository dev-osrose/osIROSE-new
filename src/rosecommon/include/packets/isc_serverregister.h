#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>
#include <isccommon.h>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister)
REGISTER_RECV_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister)
class IscServerRegister : public CRosePacket {
    private:
        static const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> init;
	public:
		IscServerRegister();
		IscServerRegister(uint8_t buffer[MAX_PACKET_SIZE]);
		IscServerRegister(Isc::ServerType serverType, const std::string &name, const std::string &addr, int32_t port, int32_t right, int32_t id);

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
		Isc::ServerType serverType_;
		std::string name_;
		std::string addr_;
		int32_t port_;
		int32_t right_;
		int32_t id_;
};

}
