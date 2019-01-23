#include "cli_whisper_chat.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliWhisperChat::CliWhisperChat() : CRosePacket(CliWhisperChat::PACKET_ID) {}

CliWhisperChat::CliWhisperChat(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_string(target)) {
        return;
    }
    if (!reader.get_string(message)) {
        return;
    }
}

void CliWhisperChat::set_target(const std::string& target) {
    this->target = target;
}

const std::string& CliWhisperChat::get_target() const {
    return target;
}

void CliWhisperChat::set_message(const std::string& message) {
    this->message = message;
}

const std::string& CliWhisperChat::get_message() const {
    return message;
}

CliWhisperChat CliWhisperChat::create(const std::string& target, const std::string& message) {
    CliWhisperChat packet;
    packet.set_target(target);
    packet.set_message(message);
    return packet;
}

CliWhisperChat CliWhisperChat::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliWhisperChat(reader);
}

std::unique_ptr<CliWhisperChat> CliWhisperChat::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliWhisperChat>(reader);
}

bool CliWhisperChat::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_string(target)) {
        return false;
    }
    if (!writer.set_string(message)) {
        return false;
    }
    return true;
}

constexpr size_t CliWhisperChat::size() {
    size_t size = 0;
    return size;
}

