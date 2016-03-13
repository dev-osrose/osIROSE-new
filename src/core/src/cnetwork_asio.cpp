/*
 * cnetwork_asio.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: ctorres
 */

#include <cstdlib>
#include <iostream>
#include <thread>
#include "logconsole.h"

#include "cnetwork_asio.h"
namespace Core {

NetworkThreadPool* NetworkThreadPool::instance_ = nullptr;

CNetwork_Asio::CNetwork_Asio()
    : INetwork(),
      log_("CNetwork_ASIO"),
      networkService_(&NetworkThreadPool::GetInstance()),
      socket_(*networkService_->Get_IO_Service()),
      listener_(*networkService_->Get_IO_Service()),
      packet_offset_(0),
      packet_size_(6),
      active_(true) {}

CNetwork_Asio::~CNetwork_Asio() {
  Shutdown();

  do {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while (!discard_queue_.empty());
}

bool CNetwork_Asio::Init(std::string _ip, uint16_t _port) {
  if (_ip.length() < 2)  // We can actually use hostnames instead of IP
    // addresses. Ex. google.com
    return false;

  network_ip_address = _ip;
  network_port_ = _port;
  return true;
}

bool CNetwork_Asio::Shutdown() {
  active_ = false;
  Disconnect();

  if (listener_.is_open()) {
    //    networkService_->Get_IO_Service()->post([this]() {
    std::error_code ignored;
    listener_.close(ignored);
    //    });
  }
  return true;
}

bool CNetwork_Asio::Connect() {
  tcp::resolver resolver(*networkService_->Get_IO_Service());
  auto endpoint_iterator =
      resolver.resolve(network_ip_address, std::to_string(network_port_));

  OnConnect();
  asio::async_connect(
      socket_, endpoint_iterator,
      [this](std::error_code errorCode, const asio::ip::tcp::endpoint) {
        if (!errorCode) {
          OnConnected();
        }
      });
  active_ = true;
  return true;
}

bool CNetwork_Asio::Listen() {
  OnListen();
  tcp::endpoint endpoint(tcp::v4(), network_port_);
  listener_.open(endpoint.protocol());
  listener_.set_option(tcp::acceptor::reuse_address(true));
  listener_.non_blocking(true);
  listener_.bind(endpoint);
  listener_.listen();
  log_.icprintf("Listening started on %s:%i\n", GetIpAddress().c_str(),
                GetPort());
  active_ = true;
  AcceptConnection();
  OnListening();
  return true;
}

bool CNetwork_Asio::Reconnect() {
  Disconnect();
  Connect();
  return true;
}

bool CNetwork_Asio::Disconnect() {
  OnDisconnect();
  //  networkService_->Get_IO_Service()->post([this]() {
  std::error_code ignored;
  socket_.shutdown(asio::socket_base::shutdown_both, ignored);
  OnDisconnected();
  //  });
  return true;
}

bool CNetwork_Asio::Send(std::unique_ptr<uint8_t> _buffer) {
  std::lock_guard<std::mutex> lock(send_mutex_);
  uint8_t* raw_ptr = _buffer.get();
  uint16_t _size = (uint16_t)raw_ptr[0];
  uint16_t _command = (uint16_t)raw_ptr[2];

  discard_mutex_.lock();
  discard_queue_.push(std::move(_buffer));
  raw_ptr = discard_queue_.back().get();
  discard_mutex_.unlock();

  if (OnSend(raw_ptr))
    asio::async_write(socket_, asio::buffer(raw_ptr, _size),
                      [this](const asio::error_code& error,
                             std::size_t bytes_transferred) {
      (void)bytes_transferred;
      if (!error) {
        OnSent();
      }
      discard_mutex_.lock();
      {
        std::unique_ptr<uint8_t> _buffer = std::move(discard_queue_.front());
        discard_queue_.pop();
        _buffer.reset(nullptr);
      }
      discard_mutex_.unlock();

    });
  else
    log_.eicprintf(CL_RESET "[%d] Not sending packet: Header[%i, 0x%X]\n",
                   GetId(), _size, _command);
  return true;
}

bool CNetwork_Asio::Recv(uint16_t _size /*= 6*/) {
  OnReceive();
  {
    (void)_size;

    std::error_code errorCode;
    int16_t BytesToRead = packet_size_ - packet_offset_;
    asio::async_read(socket_,
                     asio::buffer(&buffer_[packet_offset_], BytesToRead),
                     asio::transfer_exactly(
                         BytesToRead),  // We want at least 6 bytes of data
                     [this](std::error_code errorCode, std::size_t length) {
      packet_offset_ += length;
      if (!errorCode || errorCode.value() == 11) {
        if (OnReceived() == false) {
          log_.eicprintf(
              CL_RESET
              "[%d] Something bad happened in OnReceived... Shutting "
              "down...\n" CL_RESET,
              GetId());
          Shutdown();
        }
      } else {
        if (errorCode.value() == 2) {
          log_.icprintf(CL_RESET CL_WHITE "[%d] Client disconnected.\n",
                        GetId());
          OnDisconnected();
          Shutdown();
        } else {
          log_.eicprintf(
              CL_RESET CL_WHITE
              "[%d] Error occurred[CNetwork_Asio::Recv:%i]: %s\n" CL_RESET,
              GetId(), errorCode.value(), errorCode.message().c_str());

          Shutdown();
          return;
        }
      }
      recv_condition_.notify_all();
      if (active_) Recv();
    });
  }
  return true;
}

void CNetwork_Asio::AcceptConnection() {
  listener_.async_accept([this](std::error_code ec, tcp::socket socket) {
    if (!ec) {
      if (this->OnAccept())  // This should be changed to use a client session
      // instead of a CNetwork_Asio class
      {
        socket.non_blocking(true);
        // Do something here for the new connection.
        // Make sure to use std::move(socket)
        // std::make_shared<CClientSesson>( std::move(socket) );
        this->OnAccepted(std::move(socket));
      } else {
        // Kill the socket
        std::error_code ignored;
        socket.close(ignored);
      }
    }
    if (active_) AcceptConnection();
  });
}

bool CNetwork_Asio::OnConnect() { return true; }

void CNetwork_Asio::OnConnected() {
  //  if (!listener_.is_open()) Recv();
}

bool CNetwork_Asio::OnListen() { return true; }

void CNetwork_Asio::OnListening() {
  //  if (!listener_.is_open()) Recv();
}

bool CNetwork_Asio::OnDisconnect() { return true; }

void CNetwork_Asio::OnDisconnected() {}

bool CNetwork_Asio::OnReceive() { return true; }

bool CNetwork_Asio::OnReceived() { return true; }

bool CNetwork_Asio::OnSend(uint8_t* _buffer) {
  (void)_buffer;
  return true;
}

void CNetwork_Asio::OnSent() {}

bool CNetwork_Asio::OnAccept() { return true; }

void CNetwork_Asio::OnAccepted(tcp::socket _sock) { (void)_sock; }

bool CNetwork_Asio::HandlePacket(uint8_t* _buffer) {
  (void)_buffer;
  return true;
}
}
