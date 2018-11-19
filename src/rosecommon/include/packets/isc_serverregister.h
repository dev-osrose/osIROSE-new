#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <isccommon.h>
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister)
REGISTER_SEND_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister)
class IscServerRegister : public CRosePacket {
	public:
		IscServerRegister();
		IscServerRegister(CRoseReader reader);
	private:
		IscServerRegister(Isc::ServerType serverType, const std::string& name, const std::string& addr, int32_t port, int32_t right, int32_t id);
	public:

		virtual ~IscServerRegister() = default;

		Isc::ServerType serverType() const;
		IscServerRegister& set_serverType(Isc::ServerType);
		const std::string& name() const;
		IscServerRegister& set_name(const std::string&);
		const std::string& addr() const;
		IscServerRegister& set_addr(const std::string&);
		int32_t port() const;
		IscServerRegister& set_port(int32_t);
		int32_t right() const;
		IscServerRegister& set_right(int32_t);
		int32_t id() const;
		IscServerRegister& set_id(int32_t);

		static IscServerRegister create(Isc::ServerType serverType, const std::string& name, const std::string& addr, int32_t port, int32_t right, int32_t id);
		static IscServerRegister create(uint8_t *buffer);
		static std::unique_ptr<IscServerRegister> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		Isc::ServerType serverType_;
		std::string name_;
		std::string addr_;
		int32_t port_;
		int32_t right_;
		int32_t id_;
};

}