// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * cnetwork_asio.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: ctorres
 */

#include <cstdlib>
#include <iostream>
#include <thread>
#include "cnetwork_asio.h"
#include "platform_defines.h"

namespace Core {

NetworkThreadPool* NetworkThreadPool::instance_ = nullptr;

CNetwork_Asio::CNetwork_Asio()
    : INetwork(),
      networkService_(&NetworkThreadPool::GetInstance()),
      socket_(*networkService_->Get_IO_Service()),
      listener_(*networkService_->Get_IO_Service()),
      packet_offset_(0),
      packet_size_(6),
      active_(true),
      remote_connection_(false) {
    INetwork::SetLastUpdateTime(Core::Time::GetTickCount());
  logger_ = CLog::GetLogger(log_type::NETWORK).lock();
}

CNetwork_Asio::~CNetwork_Asio() {
  CNetwork_Asio::Shutdown();

  if (process_thread_.joinable()) process_thread_.join();

  do {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while (!discard_queue_.empty());

  send_mutex_.lock();
  while (send_queue_.empty() == false) send_queue_.pop();
  send_mutex_.unlock();

  recv_mutex_.lock();
  while (recv_queue_.empty() == false) recv_queue_.pop();
  recv_mutex_.unlock();

  logger_.reset();
}

bool CNetwork_Asio::Init(std::string _ip, uint16_t _port) {
  if (_ip.length() < 2)  // We can actually use host names instead of IP
    // addresses. Ex. google.com
    return false;

  network_ip_address_ = _ip;
  network_port_ = _port;
  return true;
}

bool CNetwork_Asio::Shutdown(bool _final) {
  if (_final == true || OnShutdown() == true) {
    Disconnect();

    if (listener_.is_open()) {
      std::error_code ignored;
      listener_.close(ignored);
    }
    active_ = false;
  }
  return true;
}

bool CNetwork_Asio::Connect() {
  tcp::resolver resolver(*networkService_->Get_IO_Service());
  auto endpoint_iterator =
      resolver.resolve(network_ip_address_, std::to_string(network_port_));

  OnConnect();
  send_mutex_.lock();
  std::error_code errorCode;
  asio::connect(socket_, endpoint_iterator, errorCode);
  send_mutex_.unlock();
  remote_connection_ = true;
  if (!errorCode) {
    OnConnected();
    active_ = true;
  } else {
    active_ = false;
  }
  return active_;
}

bool CNetwork_Asio::Listen() {
  OnListen();
  tcp::endpoint endpoint(asio::ip::address::from_string(network_ip_address_), network_port_);
  listener_.open(endpoint.protocol());
  listener_.set_option(tcp::acceptor::reuse_address(true));
  listener_.non_blocking(true);
  listener_.bind(endpoint);
  listener_.listen();
  logger_->info("Listening started on {}:{}", endpoint.address().to_string(), endpoint.port());
  active_ = true;
  AcceptConnection();
  OnListening();
  return true;
}

bool CNetwork_Asio::Reconnect() {
  if (remote_connection_ == false) return false;

  Disconnect();
  return Connect();
}

bool CNetwork_Asio::Disconnect() {
  OnDisconnect();
  std::error_code ignored;
  socket_.shutdown(asio::socket_base::shutdown_both, ignored);
  OnDisconnected();
  return true;
}

void CNetwork_Asio::ProcessSend() {
  if (true == active_) {
    discard_mutex_.lock();
    bool discard_empty = discard_queue_.empty();
    discard_mutex_.unlock();

    send_mutex_.lock();
    bool send_empty = send_queue_.empty();
    send_mutex_.unlock();

    if (send_empty != true && discard_empty == true) {
      send_mutex_.lock();
      auto _buffer = std::move(send_queue_.front());
      send_queue_.pop();
      send_mutex_.unlock();

      uint8_t* raw_ptr = _buffer.get();
      uint16_t _size = *reinterpret_cast<uint16_t*>( raw_ptr );
      uint16_t _command = *reinterpret_cast<uint16_t*>( raw_ptr + sizeof(uint16_t) );
      discard_mutex_.lock();
      discard_queue_.push(std::move(_buffer));
      raw_ptr = discard_queue_.back().get();
      discard_mutex_.unlock();

#ifdef SPDLOG_TRACE_ON
      fmt::MemoryWriter out;
      logger_->trace("ProcessSend: Header[{0}, 0x{1:04x}]: ", _size, (uint16_t)_command);
      for (int i = 0; i < _size; i++)
        out.write( "0x{0:02x} ", raw_ptr[i] );
      logger_->trace( "{}", out.c_str() );
#endif

      if (OnSend(raw_ptr))
        asio::async_write(
            socket_, asio::buffer(raw_ptr, _size),
            [this](const asio::error_code& error,
                   std::size_t bytes_transferred) {
              (void)bytes_transferred;
              if (!error) {
                OnSent();
                last_update_time_ = (Core::Time::GetTickCount());
              } else {
                logger_->debug("ProcessSend: error = {}: {}", error.value(), error.message());

                switch(error.value()) {
                  case asio::error::basic_errors::connection_reset:
                  case asio::error::basic_errors::network_reset:
                    Shutdown();
                    break;
                  default:
                    logger_->warn("ProcessSend: async_write returned an error sending the packet. {}: {}", error.value(), error.message());
                    break;
                }
              }

              discard_mutex_.lock();
              {
                std::unique_ptr<uint8_t[]> _buffer =
                    std::move(discard_queue_.front());
                discard_queue_.pop();
                _buffer.reset(nullptr);
              }
              discard_mutex_.unlock();

              ProcessSend();

            });
      else
        logger_->debug("Not sending packet: [{0}, 0x{1:x}] to client {2}",
                       _size, _command, GetId());
    }
  }
}

bool CNetwork_Asio::Send(std::unique_ptr<uint8_t[]> _buffer) {
  send_mutex_.lock();
  send_queue_.push(std::move(_buffer));
  send_mutex_.unlock();

  ProcessSend();
  return true;
}

bool CNetwork_Asio::Recv(uint16_t _size /*= 6*/) {
  if (OnReceive() == true) {
    (void)_size;

    int16_t BytesToRead = packet_size_ - packet_offset_;
    asio::async_read(
        socket_, asio::buffer(&buffer_[packet_offset_], BytesToRead),
        asio::transfer_exactly(
            BytesToRead),  // We want at least 6 bytes of data
        [this](std::error_code errorCode, std::size_t length) {

          packet_offset_ += (uint16_t)length;
          last_update_time_ = (Core::Time::GetTickCount());

          if (!errorCode || errorCode.value() == 11) {
            if (OnReceived() == false) {
              logger_->debug(
                  "OnReceived aborted the connection, disconnecting...");
              Shutdown();
            }
          } else {
            if (errorCode.value() == 2 || errorCode.value() == 104) {
              logger_->info("Client {} disconnected.", GetId());
              OnDisconnected();
              Shutdown();
            } else {
              logger_->debug("Client {}: Error {}: {}", GetId(), errorCode.value(),
                             errorCode.message());
              OnDisconnected();
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
        CNetwork_Asio* nSock = new CNetwork_Asio();
        nSock->SetSocket(std::move(socket));

        this->OnAccepted(nSock);
      } else {
        // Kill the socket
        std::error_code ignored;
        socket.close(ignored);
      }
    }
    if (active_) AcceptConnection();
  });
}
}
