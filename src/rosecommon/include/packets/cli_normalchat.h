#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_NORMAL_CHAT, CliNormalChat)
REGISTER_SEND_PACKET(ePacketType::PAKCS_NORMAL_CHAT, CliNormalChat)
class CliNormalChat : public CRosePacket {
	public:
		CliNormalChat();
		CliNormalChat(CRoseReader reader);
	private:
		CliNormalChat(const std::string& message);
	public:

		virtual ~CliNormalChat() = default;

		const std::string& message() const;
		CliNormalChat& set_message(const std::string&);

		static CliNormalChat create(const std::string& message);
		static CliNormalChat create(uint8_t *buffer);
		static std::unique_ptr<CliNormalChat> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		std::string message_;
};

}