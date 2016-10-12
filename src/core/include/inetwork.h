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
      : network_id_(0),
        network_type_(0),
        network_port_(0),
        network_ip_address_("") {}
  virtual ~INetwork() {}

  /*!
   * \brief Used to set the underlying IP adrress and port used in the rest of the functions.
   *
   * This function should be called before using any other function. This sets up the class to a default working state.
   *
   * \param[in] _ip The IP Address that the network will listen on or connect to.
   * \param[in] _port The port that the network with either listen on or connect to.
  */
  virtual bool Init(std::string _ip, uint16_t _port) = 0;

  /*!
   * \brief Used to shutdown networking operations
   *
   * This function cleans up all of the packet queues and shuts down the current socket.
  */
  virtual bool Shutdown(bool _final = false) = 0;

  /*!
   * \brief Used to connect to the ip and port stored by Init
   *
   * This function opens a network connection to the stored ip and port.
   *
   * \sa Init OnConnect OnConnected
  */
  virtual bool Connect() = 0;

  /*!
   * \brief Used to open a listen socket on the ip and port stored by Init
   *
   * This function opens a networking socket to listen for connection requests.
   *
   * \sa Init OnListen OnListening
  */
  virtual bool Listen() = 0;

  /*!
   * \brief Used to reconnect to the currently connected ip and port.
   *
   * This function resets the current class back to its default state right after calling Init
   *
   * \sa Init Connect Disconnect
  */
  virtual bool Reconnect() = 0;

  /*!
   * \brief Used to close the current socket connection.
   *
   * This function disconnects the currect socket connection.
   *
   * \sa Connect OnDisconnect OnDisconnected
  */
  virtual bool Disconnect() = 0;

  /*!
   * \brief Used to set the ID for this connection.
   *
   * \param[in] _val The ID for this instance. Should not be the same as any other.
  */
  virtual void SetId(uint32_t _val) { network_id_ = _val; }
  virtual void SetType(uint32_t _val) { network_type_ = _val; }

  virtual uint32_t GetId() const { return network_id_; }
  virtual uint32_t GetType() const { return network_type_; }
  virtual uint16_t GetPort() const { return network_port_; }
  virtual std::string GetIpAddress() const { return network_ip_address_; }

 protected:
  virtual bool Send(std::unique_ptr<uint8_t[]> _buffer) = 0;
  virtual bool Recv(uint16_t _size = 6) = 0;

  // Callback functions
  virtual bool OnConnect() = 0;
  virtual void OnConnected() = 0;
  virtual bool OnListen() = 0;
  virtual void OnListening() = 0;
  virtual bool OnDisconnect() = 0;
  virtual void OnDisconnected() = 0;
  virtual bool OnReceive() = 0;
  virtual bool OnReceived() = 0;
  virtual bool OnSend(uint8_t* _buffer) = 0;
  virtual void OnSent() = 0;
  virtual bool OnShutdown() = 0;

  // private:
  uint32_t network_id_;
  uint32_t network_type_;
  uint16_t network_port_;

  std::string network_ip_address_;
};
}

#endif
