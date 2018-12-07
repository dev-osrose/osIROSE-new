#include "srv_inventory_data.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;


void SrvInventoryData::Header::set_isCreated(const unsigned int isCreated) {
    this->data.isCreated = isCreated;
}

unsigned int SrvInventoryData::Header::get_isCreated() const {
    return data.isCreated;
}

void SrvInventoryData::Header::set_id(const unsigned int id) {
    this->data.id = id;
}

unsigned int SrvInventoryData::Header::get_id() const {
    return data.id;
}

void SrvInventoryData::Header::set_type(const unsigned int type) {
    this->data.type = type;
}

unsigned int SrvInventoryData::Header::get_type() const {
    return data.type;
}

void SrvInventoryData::Header::set_header(const uint16_t header) {
    this->data.header = header;
}

uint16_t SrvInventoryData::Header::get_header() const {
    return data.header;
}

bool SrvInventoryData::Header::write(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(data.header)) {
        return false;
    }
    return true;
}

bool SrvInventoryData::Header::read(CRoseReader& reader) {
    if (!reader.get_uint16_t(data.header)) {
        return false;
    }
    return true;
}

constexpr size_t SrvInventoryData::Header::size() {
    return sizeof(data);
}


SrvInventoryData::SrvInventoryData() : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA) {}

SrvInventoryData::SrvInventoryData(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_int64_t(zuly)) {
        return;
    }
    for (size_t index = 0; index < MAX_ITEMS; ++index) {
        if (!reader.get_Item(items[index])) {
            return;
        }
    }
}

void SrvInventoryData::set_zuly(const int64_t zuly) {
    this->zuly = zuly;
}

int64_t SrvInventoryData::get_zuly() const {
    return zuly;
}

void SrvInventoryData::set_items(const Item* items) {
    for (size_t index = 0; index < MAX_ITEMS; ++index) {
        this->items[index] = items[index];
    }
}

void SrvInventoryData::set_items(const Item items, size_t index) {
    this->items[index] = items;
}

const Item* SrvInventoryData::get_items() const {
    return items;
}

Item SrvInventoryData::get_items(size_t index) const {
    return items[index];
}

SrvInventoryData SrvInventoryData::create(const int64_t& zuly) {
    SrvInventoryData packet;
    packet.set_zuly(zuly);
    return packet;
}

SrvInventoryData SrvInventoryData::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvInventoryData(reader);
}

std::unique_ptr<SrvInventoryData> SrvInventoryData::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvInventoryData>(reader);
}

void SrvInventoryData::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_int64_t(zuly)) {
        return;
    }
    for (size_t index = 0; index < MAX_ITEMS; ++index) {
        if (!writer.set_Item(items[index])) {
            return;
        }
    }
}

constexpr size_t SrvInventoryData::size() {
    size_t size = 0;
    size += sizeof(int64_t);
    size += sizeof(Item) * MAX_ITEMS;
    return size;
}

