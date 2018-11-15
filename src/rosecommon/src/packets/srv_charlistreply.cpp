#include "srv_charlistreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvCharListReply::SrvCharListReply() : CRosePacket(ePacketType::PAKCC_CHAR_LIST_REPLY) {}

SrvCharListReply::SrvCharListReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCC_CHAR_LIST_REPLY, "Not the right packet: " << to_underlying(get_type()));
	{
		uint8_t index = 0;
		reader.get_uint8_t(index);
		while (index-- > 0) {
			CharListReply::CharInfo tmp;
			reader.get_iserialize(tmp);
			characters_.push_back(tmp);
		}
	}
}

const std::vector<CharListReply::CharInfo>& SrvCharListReply::characters() const { return characters_; }

SrvCharListReply& SrvCharListReply::set_characters(const std::vector<CharListReply::CharInfo>& data) { characters_ = data; return *this; }

SrvCharListReply& SrvCharListReply::add_charinfo(const CharListReply::CharInfo& data) { characters_.push_back(data); return *this; }


void SrvCharListReply::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(characters_.size());
	for (const auto& elem : characters_) writer.set_iserialize(elem);
}

uint16_t SrvCharListReply::get_size() const {
	uint16_t size = 0;
	{ for (const auto& it : characters_) size += it.get_size();} size += sizeof(uint8_t);
	return size;
}


SrvCharListReply SrvCharListReply::create() {


	return SrvCharListReply();
}

SrvCharListReply SrvCharListReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvCharListReply(reader);
}

}