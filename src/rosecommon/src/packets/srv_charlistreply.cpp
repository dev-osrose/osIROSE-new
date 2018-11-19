#include "srv_charlistreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvCharListReply::SrvCharListReply() : CRosePacket(ePacketType::PAKCC_CHAR_LIST_REPLY) {}

SrvCharListReply::SrvCharListReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCC_CHAR_LIST_REPLY, "Not the right packet: " << to_underlying(get_type()));
	{
		uint8_t index = 0;
		reader.get_uint8_t(index);
		for (uint8_t i = 0; i < index; ++i) {
			CharListReply::CharInfo tmp;
			reader.get_iserialize(tmp);
			characters_.push_back(tmp);
		}
	}
}

const std::vector<CharListReply::CharInfo>& SrvCharListReply::characters() const { return characters_; }

SrvCharListReply& SrvCharListReply::set_characters(const std::vector<CharListReply::CharInfo>& data) { characters_ = data; return *this; }

SrvCharListReply& SrvCharListReply::add_charinfo(const CharListReply::CharInfo& data) { characters_.push_back(data); return *this; }


void SrvCharListReply::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(characters_.size());
	for (const auto& elem : characters_) writer.set_iserialize(elem);
}

SrvCharListReply SrvCharListReply::create() {


	return SrvCharListReply();
}

SrvCharListReply SrvCharListReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvCharListReply(reader);
}
std::unique_ptr<SrvCharListReply> SrvCharListReply::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvCharListReply>(reader);
}

}