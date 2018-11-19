#include "srv_globalvars.h"
#include "throwassert.h"

namespace RoseCommon {

SrvGlobalVars::SrvGlobalVars() : CRosePacket(ePacketType::PAKWC_GLOBAL_VARS) {}

SrvGlobalVars::SrvGlobalVars(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_GLOBAL_VARS, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); type_ = static_cast<ServerData::Type>(_tmp); }
	reader.get_uint32_t(counter_);
	reader.get_uint16_t(popBase_);
	reader.get_uint16_t(devBase_);
	for (size_t index = 0; index < MAX_SELL_TYPE; ++index) reader.get_uint16_t(consume_[index]);
	reader.get_uint16_t(dev_);
	reader.get_uint32_t(pop_);
	for (size_t index = 0; index < MAX_SELL_TYPE; ++index) reader.get_uint32_t(item_[index]);
}

ServerData::Type SrvGlobalVars::type() const { return type_; }

SrvGlobalVars& SrvGlobalVars::set_type(ServerData::Type data) { type_ = data; return *this; }

uint32_t SrvGlobalVars::counter() const { return counter_; }

SrvGlobalVars& SrvGlobalVars::set_counter(uint32_t data) { counter_ = data; return *this; }

uint16_t SrvGlobalVars::popBase() const { return popBase_; }

SrvGlobalVars& SrvGlobalVars::set_popBase(uint16_t data) { popBase_ = data; return *this; }

uint16_t SrvGlobalVars::devBase() const { return devBase_; }

SrvGlobalVars& SrvGlobalVars::set_devBase(uint16_t data) { devBase_ = data; return *this; }

const uint16_t* SrvGlobalVars::consume() const { return consume_; }

const uint16_t& SrvGlobalVars::consume(size_t index) const { return consume_[index]; }

SrvGlobalVars& SrvGlobalVars::set_consume(const uint16_t* data) { for (size_t index = 0; index < MAX_SELL_TYPE; ++index) consume_[index] = data[index]; return *this; }

SrvGlobalVars& SrvGlobalVars::set_consume(const uint16_t& data, size_t index) { consume_[index] = data; return *this; }

uint16_t SrvGlobalVars::dev() const { return dev_; }

SrvGlobalVars& SrvGlobalVars::set_dev(uint16_t data) { dev_ = data; return *this; }

uint32_t SrvGlobalVars::pop() const { return pop_; }

SrvGlobalVars& SrvGlobalVars::set_pop(uint32_t data) { pop_ = data; return *this; }

const uint32_t* SrvGlobalVars::item() const { return item_; }

const uint32_t& SrvGlobalVars::item(size_t index) const { return item_[index]; }

SrvGlobalVars& SrvGlobalVars::set_item(const uint32_t* data) { for (size_t index = 0; index < MAX_SELL_TYPE; ++index) item_[index] = data[index]; return *this; }

SrvGlobalVars& SrvGlobalVars::set_item(const uint32_t& data, size_t index) { item_[index] = data; return *this; }


void SrvGlobalVars::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(type_);
	writer.set_uint32_t(counter_);
	writer.set_uint16_t(popBase_);
	writer.set_uint16_t(devBase_);
	for (size_t index = 0; index < MAX_SELL_TYPE; ++index) writer.set_uint16_t(consume_[index]);
	writer.set_uint16_t(dev_);
	writer.set_uint32_t(pop_);
	for (size_t index = 0; index < MAX_SELL_TYPE; ++index) writer.set_uint32_t(item_[index]);
}

SrvGlobalVars SrvGlobalVars::create() {


	return SrvGlobalVars();
}

SrvGlobalVars SrvGlobalVars::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvGlobalVars(reader);
}
std::unique_ptr<SrvGlobalVars> SrvGlobalVars::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvGlobalVars>(reader);
}

}