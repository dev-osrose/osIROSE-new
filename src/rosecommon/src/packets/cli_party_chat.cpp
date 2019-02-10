#include "cli_party_chat.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliPartyChat::CliPartyChat() : CRosePacket(CliPartyChat::PACKET_ID) {}

CliPartyChat::CliPartyChat(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_string(message)) {
        return;
    }
}

void CliPartyChat::set_message(const std::string& message) {
    this->message = message;
}

const std::string& CliPartyChat::get_message() const {
    return message;
}

CliPartyChat CliPartyChat::create(const std::string& message) {
    CliPartyChat packet;
    packet.set_message(message);
    return packet;
}

CliPartyChat CliPartyChat::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliPartyChat(reader);
}

std::unique_ptr<CliPartyChat> CliPartyChat::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliPartyChat>(reader);
}

bool CliPartyChat::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_string(message)) {
        return false;
    }
    return true;
}

constexpr size_t CliPartyChat::size() {
    size_t size = 0;
    return size;
}

