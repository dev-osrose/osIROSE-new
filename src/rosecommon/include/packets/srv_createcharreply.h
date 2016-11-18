#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKCC_CREATE_CHAR_REPLY, SrvCreateCharReply)
class SrvCreateCharReply : public CRosePacket {
	public:
        enum eResult : uint8_t {
            OK = 0,
            FAILED,
            NAME_TAKEN,
            INVALID_VALUE,
            TOO_MANY_CHARS,
            BLOCKED
        };

		SrvCreateCharReply();
		SrvCreateCharReply(SrvCreateCharReply::eResult result, uint8_t platinium = 0);

		virtual ~SrvCreateCharReply() = default;

		SrvCreateCharReply::eResult result() const;
		uint8_t platinium() const;

	protected:
		virtual void pack() override;

	private:
		SrvCreateCharReply::eResult result_;
		uint8_t platinium_;
};

}
