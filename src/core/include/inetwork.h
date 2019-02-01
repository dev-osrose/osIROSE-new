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

/*!
 * \file inetwork.h
 *
 * \author RavenX8
 * \date nov 2015
 *
 * The interface definition for the network classes
 */
#ifndef INETWORK_H_
#define INETWORK_H_

#include <stdint.h>
#include <memory>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>

#include "platform_defines.h"

namespace Core {

/*!
 * \class INetwork
 *
 * \brief An interface for networking sockets
 *
 * This interface is used to define the basic need for the server networking.
 *
 * \author Raven
 * \date nov 2015
 */
class INetwork {
 public:
  INetwork()
      : OnConnect(nullptr),
        OnConnected(nullptr),
        OnListen(nullptr),
        OnListening(nullptr),
        OnDisconnect(nullptr),
        OnDisconnected(nullptr),
        OnReceive(nullptr),
        OnReceived(nullptr),
        OnSend(nullptr),
        OnSent(nullptr),
        OnShutdown(nullptr),
        network_id_(0),
        network_type_(0),
        network_port_(0),
        socket_id_(0),
        network_name_(""),
        network_address_(""),
        update_time_(Core::Time::GetTickCount()) {
    initCallbacks();
  }
  virtual ~INetwork() {}

  /*!
   * \brief Used to set the underlying IP adrress and port used in the rest of the functions.
   *
   * This function should be called before using any other function. This sets up the class to a default working state.
   *
   * \param[in] _ip The IP Address that the network will listen on or connect to.
   * \param[in] _port The port that the network with either listen on or connect to.
  */
  virtual bool init(std::string _ip, uint16_t _port) = 0;

  /*!
   * \brief Used to shutdown networking operations
   *
   * This function cleans up all of the packet queues and shuts down the current socket.
  */
  virtual bool shutdown(bool _final = false) = 0;

  /*!
   * \brief Used to connect to the ip and port stored by Init
   *
   * This function opens a network connection to the stored ip and port.
   *
   * \sa Init OnConnect OnConnected
  */
  virtual bool connect() = 0;

  /*!
   * \brief Used to open a listen socket on the ip and port stored by Init
   *
   * This function opens a networking socket to listen for connection requests.
   *
   * \sa Init OnListen OnListening
  */
  virtual bool listen() = 0;

  /*!
   * \brief Used to reconnect to the currently connected ip and port.
   *
   * This function resets the current class back to its default state right after calling Init
   *
   * \sa Init connect disconnect
  */
  virtual bool reconnect() = 0;

  /*!
   * \brief Used to close the current socket connection.
   *
   * This function disconnects the currect socket connection.
   *
   * \sa connect OnDisconnect OnDisconnected
  */
  virtual bool disconnect() = 0;

  virtual bool is_active() const = 0;
  virtual void set_active(bool _val) = 0;

  /*!
   * \brief Used to set the ID for this connection.
   *
   * \param[in] _val The ID for this instance. Should not be the same as any other.
  */
  virtual void set_id(uint32_t _val) { network_id_ = _val; }
  virtual void set_type(uint32_t _val) { network_type_ = _val; }
  virtual void set_port(uint16_t _val) { network_port_ = _val; }
  virtual void set_socket_id(uint16_t _val) { socket_id_ = _val; }
  virtual void set_name(const std::string _val) { network_name_ = _val; }
  virtual void set_address(std::string _val) { network_address_ = _val; }
  virtual void set_update_time(const std::chrono::steady_clock::time_point time) { update_time_ = time; }

  virtual uint32_t get_id() const { return network_id_; }
  virtual uint32_t get_type() const { return network_type_; }
  virtual uint16_t get_port() const { return network_port_; }
  virtual uint16_t get_socket_id() const { return socket_id_; }
  virtual std::string get_name() const { return network_name_; }
  virtual std::string get_address() const { return network_address_; }
  virtual std::chrono::steady_clock::time_point get_update_time() const { return update_time_; }

  void registerOnAccept(std::function<bool()> _val) { OnAccept = _val; }
  void registerOnAccepted(std::function<void(std::unique_ptr<Core::INetwork>)> _val) { OnAccepted = _val; }
  void registerOnConnect(std::function<bool()> _val) { OnConnect = _val; }
  void registerOnConnected(std::function<void()> _val) { OnConnected = _val; }
  void registerOnListen(std::function<bool()> _val) { OnListen = _val; }
  void registerOnListening(std::function<void()> _val) { OnListening = _val; }
  void registerOnDisconnect(std::function<bool()> _val) { OnDisconnect = _val; }
  void registerOnDisconnected(std::function<void()> _val) { OnDisconnected = _val; }
  void registerOnReceive(std::function<bool()> _val) { OnReceive = _val; }
  void registerOnReceived(std::function<bool(uint16_t, uint16_t&, uint8_t*)> _val) { OnReceived = _val; }
  void registerOnSend(std::function<bool(uint16_t, uint8_t*)> _val) { OnSend = _val; }
  void registerOnSent(std::function<void()> _val) { OnSent = _val; }
  void registerOnShutdown(std::function<bool()> _val) { OnShutdown = _val; }

  void initCallbacks() {
    std::function<void()> fnDummyVoid = []() {};
    std::function<bool()> fnDummyBool = []() { return true; };
    std::function<void(std::unique_ptr<Core::INetwork>)> fnDummyAccepted = [](std::unique_ptr<Core::INetwork>) {};
    std::function<bool(uint16_t, uint8_t*)> fnDummySend = [](uint16_t, uint8_t*) { return true; };
    std::function<bool(uint16_t, uint16_t&, uint8_t*)> fnDummyRecv = [](uint16_t, uint16_t&, uint8_t*) {
      std::cout << "Shit is broken!\n";
      return true;
    };

    OnConnected = OnListening = OnDisconnected = OnSent = fnDummyVoid;
    OnAccept = OnConnect = OnListen = OnDisconnect = OnReceive = OnShutdown = fnDummyBool;
    OnAccepted = fnDummyAccepted;
    OnSend = fnDummySend;
    OnReceived = fnDummyRecv;
  }

  std::thread process_thread_; //TODO:: Do this correctly

  virtual bool send_data(std::unique_ptr<uint8_t[]> _buffer) = 0;
  virtual bool recv_data(uint16_t _size = 6) = 0;
  virtual void reset_internal_buffer() = 0;
  virtual void dispatch(std::function<void()> _handler) = 0;

  // Callback functions
  std::function<bool()> OnAccept;
  std::function<void(std::unique_ptr<Core::INetwork>)> OnAccepted;
  std::function<bool()> OnConnect;
  std::function<void()> OnConnected;
  std::function<bool()> OnListen;
  std::function<void()> OnListening;
  std::function<bool()> OnDisconnect;
  std::function<void()> OnDisconnected;
  std::function<bool()> OnReceive;
  std::function<bool(uint16_t, uint16_t&, uint8_t*)> OnReceived;
  std::function<bool(uint16_t, uint8_t*)> OnSend;
  std::function<void()> OnSent;
  std::function<bool()> OnShutdown;

  protected:
  uint32_t network_id_;
  uint32_t network_type_;
  uint16_t network_port_;
  uint16_t socket_id_;

  std::string network_name_;
  std::string network_address_;
  std::chrono::steady_clock::time_point update_time_;
};
}

#endif
