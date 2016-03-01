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
      active_(true) {
  process_thread_ = std::thread([this]() { Run(); });
}

CNetwork_Asio::~CNetwork_Asio() {
  Shutdown();
  process_thread_.join();
}

bool CNetwork_Asio::Run() {
  while (active_ == true) {
    // AcceptConnection();
    ProcessSend();
    ProcessRecv();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  return true;
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
  std::lock_guard<std::mutex> lock(send_mutex_);
  std::lock_guard<std::mutex> lock2(discard_mutex_);
  active_ = false;
  Disconnect();

  if (listener_.is_open()) {
    networkService_->Get_IO_Service()->post([this]() {
      std::error_code ignored;
      listener_.close(ignored);
    });
  }

  while (!send_queue_.empty()) {
    uint8_t* _buffer = std::move(send_queue_.front());
    send_queue_.pop();
    delete _buffer;
  }

  while (!discard_queue_.empty()) {
    uint8_t* _buffer = std::move(discard_queue_.front());
    send_queue_.pop();
    delete _buffer;
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
  networkService_->Get_IO_Service()->dispatch([this]() {
    std::error_code ignored;
    socket_.shutdown(asio::socket_base::shutdown_both, ignored);
    OnDisconnected();
  });
  return true;
}

bool CNetwork_Asio::Send(uint8_t* _buffer) {
  std::lock_guard<std::mutex> lock(send_mutex_);
  send_queue_.push(_buffer);
  return true;
}

bool CNetwork_Asio::Recv(uint16_t _size /*= 6*/) {
  OnReceive();
  {
    (void)_size;

    std::error_code errorCode;
    int16_t BytesToRead = packet_size_ - packet_offset_;
    asio::async_read(
        socket_, asio::buffer(&buffer_[packet_offset_], BytesToRead),
        asio::transfer_exactly(
            BytesToRead),  // We want at least 6 bytes of data
        [this](std::error_code errorCode, std::size_t length) {
          packet_offset_ += length;
          if (!errorCode || errorCode.value() == 11) {
            if (OnReceived() == false) {
              log_.eicprintf(CL_RESET
                             "Something bad happened in OnReceived... Shutting "
                             "down...\n" CL_RESET);
              Shutdown();
            }
          } else {
            if (errorCode.value() == 2) {
              log_.icprintf(CL_RESET CL_WHITE "Client disconnected.\n");
              OnDisconnected();
              Shutdown();
            } else {
              log_.eicprintf(
                  CL_RESET CL_WHITE
                  "Error occurred[CNetwork_Asio::Recv:%i]: %s\n" CL_RESET,
                  errorCode.value(), errorCode.message().c_str());

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
  //   std::error_code ec;
  //   tcp::socket socket(*networkService_->Get_IO_Service());
  //   listener_.accept(socket, ec);
  //   if (!ec) {
  //     if (this->OnAccept())
  //     {
  //       socket.non_blocking (true);
  //       this->OnAccepted(std::move(socket));
  //     } else {
  //       std::error_code ignored;
  //       socket.close(ignored);
  //     }
  //   }
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

void CNetwork_Asio::ProcessSend() {
  if (socket_.is_open()) {
    // Loop though all of Send Queue
    std::lock_guard<std::mutex> lock(send_mutex_);

    while (!send_queue_.empty()) {
      uint8_t* _buffer = std::move(send_queue_.front());
      send_queue_.pop();
      uint16_t _size = (uint16_t)_buffer[0];
      uint16_t _command = (uint16_t)_buffer[2];

      if (OnSend(_buffer))
        asio::async_write(socket_, asio::buffer(_buffer, _size),
                          [this](const asio::error_code& error,
                                 std::size_t bytes_transferred) {
                            OnSent();

                            discard_mutex_.lock();
                            {
                              uint8_t* _buffer =
                                  std::move(discard_queue_.front());
                              discard_queue_.pop();
                              delete _buffer;
                              _buffer = nullptr;
                            }
                            discard_mutex_.unlock();
                          });
      else
        log_.eicprintf(CL_RESET "Not sending packet: Header[%i, 0x%X]\n", _size,
                       _command);

      discard_mutex_.lock();
      discard_queue_.push(_buffer);
      discard_mutex_.unlock();
      // std::this_thread::sleep_for(std::chrono::milliseconds(1));
      // delete _buffer;
      //_buffer = nullptr;
      // OnSent();
    }
  }
}

void CNetwork_Asio::ProcessRecv() {
  // Loop though all of m_RecvQueue
  //  if (socket_.is_open()) Recv();
}

bool CNetwork_Asio::HandlePacket(uint8_t* _buffer) {
  (void)_buffer;
  return true;
}
}
