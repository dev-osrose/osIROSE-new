#include "client.h"
#include "packetfactory.h"
#include "cli_accept_req.h"


using namespace RoseCommon;
using namespace RoseCommon::Packet;


Client::Client(std::unique_ptr<Core::INetwork> sock) :
    CRoseSocket(std::move(sock)) {
    socket_[SocketType::Client]->registerOnConnected(std::bind(&Client::onConnected, this));
    socket_[SocketType::Client]->registerOnSend(std::bind(&Client::onSend, this, std::placeholders::_1, std::placeholders::_2));
    socket_[SocketType::Client]->registerOnReceived(std::bind(&Client::onReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}


void Client::onConnected() {
    logger_->trace("Client::OnConnected");
    auto packet = CliAcceptReq();
    send(packet);
}

bool Client::onSend(uint16_t socketId, uint8_t *buffer) {
    logger_->trace("Client::OnSend");
    (void)buffer;
#ifndef DISABLE_CRYPT
    crypt_[socketId].encodeClientPacket(buffer);
#endif
    return true;
}

bool Client::onReceived(uint16_t socketId, uint16_t& packetSize, uint8_t *buffer) {
    logger_->trace("Client::OnReceived");
    if (packetSize == 6) {
#ifndef DISABLE_CRYPT
        packetSize = crypt_[socketId].decodeServerHeader(reinterpret_cast<unsigned char*>(buffer));
#else
        packetSize = buffer[0];
#endif
        if (packetSize < 6 || packetSize > MAX_PACKET_SIZE) {
            logger_->debug("Server sent incorrect block header");
            socket_[SocketType::Client]->reset_internal_buffer();
            return false;
        }

        if (packetSize > 6)
            return true;
    }
#ifndef DISABLE_CRYPT
    if (!crypt_[socketId].decodeServerBody(reinterpret_cast<unsigned char*>(buffer))) {
        logger_->debug("Server sent an illegal block");
        socket_[SocketType::Client]->reset_internal_buffer();
        return false;
    }
#endif
    logger_->trace("Received a packet on CRoseSocket {2}: Header [{0}, 0x{1:04x}]", CRosePacket::size(buffer), (uint16_t)CRosePacket::type(buffer), get_id());
#ifdef SPDLOG_TRACE_ON
    fmt::MemoryWriter out;
    for (int i = 0; i < CRosePacket::size(buffer); ++i)
        out.write("0x{0:02x} ", buffer[i]);
    logger_->trace("{}", out.c_str());
#endif
    auto res = std::make_unique<uint8_t[]>(CRosePacket::size(buffer));
    std::memcpy(res.get(), buffer, CRosePacket::size(buffer));
    recv_mutex_.lock();
    recv_queue_.push(std::move(res));
    recv_mutex_.unlock();
    socket_[SocketType::Client]->dispatch([this]() {
            if (socket_[SocketType::Client]->is_active()) {
                recv_mutex_.lock();
                if (!recv_queue_.empty()) {
                    std::unique_ptr<uint8_t[]> buffer = std::move(recv_queue_.front());
                    recv_queue_.pop();
                    if (!handlePacket(buffer.get())) {
                        logger_->debug("HandlePacket returned false, disconnecting server");
                        socket_[SocketType::Client]->shutdown();
                    }
                }
                recv_mutex_.unlock();
            }
            });
    socket_[SocketType::Client]->reset_internal_buffer();
    return true;
}