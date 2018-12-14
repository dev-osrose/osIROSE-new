#include "cli_login_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

CliLoginReq::Password::Password() : is_valid(false) {}

CliLoginReq::Password::Password(std::string data) : password(data), is_valid(false) {
    bool valid = true;
    if (password.size() > 32) {
        password.resize(32);
        valid &= true;
    } else {
        valid &= true;
    }
    is_valid = valid;
}

bool CliLoginReq::Password::read(CRoseReader& reader) {
    bool valid = true;
    if (!reader.get_string(password, 32)) {
        return false;
    } else {
        valid &= true;
    }
    is_valid = valid;
    return true;
}

bool CliLoginReq::Password::write(CRoseBasePolicy& writer) const {
    if (!writer.set_string(password, 32)) {
        return false;
    }
    return true;
}

constexpr size_t CliLoginReq::Password::size() {
    size_t size = 0;
    size += 32;
    return size;
}



CliLoginReq::CliLoginReq() : CRosePacket(CliLoginReq::PACKET_ID) {}

CliLoginReq::CliLoginReq(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_iserialize(password)) {
        return;
    }
    if (!reader.get_string(username)) {
        return;
    }
}

void CliLoginReq::set_password(const CliLoginReq::Password& password) {
    this->password = password;
}

const CliLoginReq::Password& CliLoginReq::get_password() const {
    return password;
}

void CliLoginReq::set_username(const std::string& username) {
    this->username = username;
}

const std::string& CliLoginReq::get_username() const {
    return username;
}

CliLoginReq CliLoginReq::create(const CliLoginReq::Password& password, const std::string& username) {
    CliLoginReq packet;
    packet.set_password(password);
    packet.set_username(username);
    return packet;
}

CliLoginReq CliLoginReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliLoginReq(reader);
}

std::unique_ptr<CliLoginReq> CliLoginReq::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliLoginReq>(reader);
}

void CliLoginReq::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_iserialize(password)) {
        return;
    }
    if (!writer.set_string(username)) {
        return;
    }
}

constexpr size_t CliLoginReq::size() {
    size_t size = 0;
    size += Password::size();
    return size;
}

