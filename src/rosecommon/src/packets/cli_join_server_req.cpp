#include "cli_join_server_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

CliJoinServerReq::Password::Password() : is_valid(false) {}

CliJoinServerReq::Password::Password(std::string data) : password(data), is_valid(false) {
    bool valid = true;
    if (password.size() > 32) {
        password.resize(32);
        valid &= true;
    } else {
        valid &= true;
    }
    is_valid = valid;
}

bool CliJoinServerReq::Password::read(CRoseReader& reader) {
    bool valid = true;
    if (!reader.get_string(password, 32)) {
        return false;
    } else {
        valid &= true;
    }
    is_valid = valid;
    return true;
}

bool CliJoinServerReq::Password::write(CRoseBasePolicy& writer) const {
    if (!writer.set_string(password, 32)) {
        return false;
    }
    return true;
}

constexpr size_t CliJoinServerReq::Password::size() {
    size_t size = 0;
    size += 32;
    return size;
}



CliJoinServerReq::CliJoinServerReq() : CRosePacket(ePacketType::PAKCS_JOIN_SERVER_REQ) {}

CliJoinServerReq::CliJoinServerReq(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint32_t(sessionId)) {
        return;
    }
    if (!reader.get_iserialize(password)) {
        return;
    }
}

void CliJoinServerReq::set_sessionId(const uint32_t sessionId) {
    this->sessionId = sessionId;
}

uint32_t CliJoinServerReq::get_sessionId() const {
    return sessionId;
}

void CliJoinServerReq::set_password(const CliJoinServerReq::Password& password) {
    this->password = password;
}

const CliJoinServerReq::Password& CliJoinServerReq::get_password() const {
    return password;
}

CliJoinServerReq CliJoinServerReq::create(const uint32_t& sessionId, const CliJoinServerReq::Password& password) {
    CliJoinServerReq packet;
    packet.set_sessionId(sessionId);
    packet.set_password(password);
    return packet;
}

CliJoinServerReq CliJoinServerReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliJoinServerReq(reader);
}

void CliJoinServerReq::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint32_t(sessionId)) {
        return;
    }
    if (!writer.set_iserialize(password)) {
        return;
    }
}

constexpr size_t CliJoinServerReq::size() {
    size_t size = 0;
    size += sizeof(uint32_t);
    size += Password::size();
    return size;
}

