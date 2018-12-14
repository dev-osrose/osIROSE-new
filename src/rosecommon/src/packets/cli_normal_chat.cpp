#include "cli_normal_chat.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliNormalChat::CliNormalChat() : CRosePacket(CliNormalChat::PACKET_ID) {}

CliNormalChat::CliNormalChat(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_string(message)) {
        return;
    }
}

void CliNormalChat::set_message(const std::string& message) {
    this->message = message;
}

const std::string& CliNormalChat::get_message() const {
    return message;
}

CliNormalChat CliNormalChat::create(const std::string& message) {
    CliNormalChat packet;
    packet.set_message(message);
    return packet;
}

CliNormalChat CliNormalChat::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliNormalChat(reader);
}

std::unique_ptr<CliNormalChat> CliNormalChat::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliNormalChat>(reader);
}

void CliNormalChat::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_string(message)) {
        return;
    }
}

constexpr size_t CliNormalChat::size() {
    size_t size = 0;
    return size;
}

