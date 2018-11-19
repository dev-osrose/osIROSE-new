#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>
#include <vector>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCC_CHAR_LIST_REPLY, SrvCharListReply)
REGISTER_SEND_PACKET(ePacketType::PAKCC_CHAR_LIST_REPLY, SrvCharListReply)
class SrvCharListReply : public CRosePacket {
	public:
		SrvCharListReply();
		SrvCharListReply(CRoseReader reader);
	public:

		virtual ~SrvCharListReply() = default;

		const std::vector<CharListReply::CharInfo>& characters() const;
		SrvCharListReply& set_characters(const std::vector<CharListReply::CharInfo>&);
		SrvCharListReply& add_charinfo(const CharListReply::CharInfo&);

		static SrvCharListReply create();
		static SrvCharListReply create(uint8_t *buffer);
		static std::unique_ptr<SrvCharListReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		std::vector<CharListReply::CharInfo> characters_;
};

}