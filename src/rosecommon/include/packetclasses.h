/*!
 * \file packetclasses.h
 *
 * \author L3nn0x RavenX8
 * \date april 2016
 *
 * This file contains all packet subclasses and the logic needed
 */
#ifndef _PACKETCLASSES_H_
#define _PACKETCLASSES_H_

#include "epackettype.h"
#include "crosepacket.h"
#include <string>
#include <exception>
#include <vector>
#include "iscpackets.pb.h"

namespace RoseCommon {

//---------------------------------------------------
// Server Recv packets
//---------------------------------------------------
class CliAlive : public CRosePacket {
public:
  CliAlive(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_ALIVE)
      throw std::runtime_error("Not the right packet!");
  }
  CliAlive() : CRosePacket(ePacketType::PAKCS_ALIVE) {}

  virtual ~CliAlive() {}
};

class CliAcceptReq : public CRosePacket {
public:
  CliAcceptReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_ACCEPT_REQ)
      throw std::runtime_error("Not the right packet!");
  }
  CliAcceptReq() : CRosePacket(ePacketType::PAKCS_ACCEPT_REQ) {}

  virtual ~CliAcceptReq() {}
};

class CliLoginReq : public CRosePacket {
 public:
  CliLoginReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_LOGIN_REQ)
      throw std::runtime_error("Not the right packet!");
    char pass[32];
    *this >> pass >> username_;
    password_ = std::string(pass, 32);
  }
  CliLoginReq(const std::string &user, const std::string &pass)
      : CRosePacket(ePacketType::PAKCS_LOGIN_REQ),
        password_(pass),
        username_(user) {}

  virtual ~CliLoginReq() {}

  std::string password() const { return password_; }
  std::string username() const { return username_; }

 protected:
  void pack() { *this << password_.c_str() << username_; }

 private:
  std::string password_;
  std::string username_;
};

class CliChannelReq : public CRosePacket {
 public:
  CliChannelReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_CHANNEL_LIST_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> server_id_;
  }
  CliChannelReq(uint32_t server_id) : CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ), server_id_(server_id) {}

  virtual ~CliChannelReq() {}

  uint32_t server_id() const { return server_id_; }

protected:
  void pack() { *this << server_id_; }

 private:
  uint32_t server_id_;
};

class CliServerSelectReq : public CRosePacket {
 public:
  CliServerSelectReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_SRV_SELECT_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> server_id_ >> channel_id_;
  }

  CliServerSelectReq(uint32_t server_id, uint8_t channel_id) : CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ), server_id_(server_id), channel_id_(channel_id) {}

  virtual ~CliServerSelectReq() {}

  uint32_t server_id() const { return server_id_; }
  uint8_t channel_id() const { return channel_id_; }

protected:
  void pack() { *this << server_id_ << channel_id_; }

 private:
  uint32_t server_id_;
  uint8_t channel_id_;
};

class CliJoinServerReq : public CRosePacket {
 public:
  CliJoinServerReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_LOGIN_REQ)
      throw std::runtime_error("Not the right packet!");
    char pass[32];
    *this >> channel_id_ >> pass;
    password_ = std::string(pass, 32);
  }
  CliJoinServerReq(uint32_t id, const std::string &pass)
      : CRosePacket(ePacketType::PAKCS_LOGIN_REQ),
        channel_id_(id),
        password_(pass) {}

  virtual ~CliJoinServerReq() {}

  std::string password() const { return password_; }
  uint32_t channel_id() const { return channel_id_; }

 protected:
  void pack() { *this << channel_id_ << password_.c_str(); }

 private:
  uint32_t channel_id_;
  std::string password_;
};

//-----------------------------------------------
// Send Packets
//-----------------------------------------------

class SrvAcceptClient : public CRosePacket {
 public:
  SrvAcceptClient(uint32_t rand_value, uint8_t result_ = 0x02)
      : CRosePacket(ePacketType::PAKSS_ACCEPT_REPLY),
        result_(result_),
        rand_value_(rand_value) {}

 protected:
  void pack() { *this << result_ << rand_value_; }

 private:
  uint8_t result_;
  uint32_t rand_value_;
};

class SrvLoginReply : public CRosePacket {
 public:
  SrvLoginReply(uint8_t result, uint16_t right, uint16_t type)
      : CRosePacket(ePacketType::PAKLC_LOGIN_REPLY),
        right_(right),
        type_(type),
        result_(result) {}

  virtual ~SrvLoginReply() {}

  uint8_t &result() { return result_; }
  uint16_t &right() { return right_; }
  uint16_t &type() { return type_; }

  void setRight(uint16_t right) { right_ = right; }
  void addServer(const std::string &name, uint32_t id, bool isTest = false) {
    info channel(name, id, isTest);
    channel_list_.push_back(channel);
  }

  enum eResult : uint8_t {
    OK = 0,
    FAILED,
    UNKNOWN_ACCOUNT,
    INVALID_PASSWORD,
    ALREADY_LOGGEDIN,
    REFUSED_ACCOUNT,
    NEED_CHARGE,
    NO_RIGHT_TO_CONNECT,
    TOO_MANY_USERS,
    NO_NAME,
    INVALID_VERSION,
    OUTSIDE_REGION
  };

 protected:
  void pack() {
    *this << result_ << right_ << type_;

    for (auto &server : channel_list_)
    {
      char pad = ' ';
      if(server.test_ == true)
        pad = '@';
      *this << pad << server.name_ << server.channel_id_;
    }
  }

 private:
  uint16_t right_;
  uint16_t type_;
  uint8_t result_;

  struct info {
    std::string name_;
    uint32_t channel_id_;
    bool test_;

    info(const std::string &name, uint32_t id, bool isTest = false)
        : name_(name), channel_id_(id), test_(isTest) {}
  };

  std::vector<info> channel_list_;
};

class SrvChannelReply : public CRosePacket {
 public:
  SrvChannelReply(uint32_t serverid)
      : CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY),
        server_id_(serverid),
        channel_count_(0) {}

  virtual ~SrvChannelReply() {}

  uint8_t server_id() const { return server_id_; }
  uint16_t channel_count() const { return channel_count_; }

  void addChannel(const std::string &name, uint8_t id,
                  uint16_t user_capacity_percentage, uint8_t low_age = 0,
                  uint8_t high_age = 0) {
    ++channel_count_;
    info channel(name, id, user_capacity_percentage, low_age, high_age);
    channel_list_.push_back(channel);
  }

 protected:
  void pack() {
    *this << server_id_ << channel_count_;

    for (auto &server : channel_list_)
      *this << server.channel_id_ << server.low_age_ << server.high_age_
            << server.capacity_ << server.name_;
  }

 private:
  uint32_t server_id_;
  uint8_t channel_count_;

  struct info {
    std::string name_;
    uint8_t channel_id_;
    uint8_t low_age_;
    uint8_t high_age_;
    uint16_t capacity_;

    info(const std::string &name, uint8_t id, uint16_t user_capacity_percentage,
         uint8_t low_age = 0, uint8_t high_age = 0)
        : name_(name),
          channel_id_(id),
          low_age_(low_age),
          high_age_(high_age),
          capacity_(user_capacity_percentage) {}
  };

  std::vector<info> channel_list_;
};

class SrvServerSelectReply : public CRosePacket {
 public:
  SrvServerSelectReply(const std::string &ip, uint32_t client_id,
                       uint32_t crypt_val, uint16_t port)
      : CRosePacket(ePacketType::PAKLC_SRV_SELECT_REPLY),
        client_id_(client_id),
        crypt_val_(crypt_val),
        port_(port),
        result_(0),
        ip_(ip) {}

  virtual ~SrvServerSelectReply() {}

  uint32_t client_id() const { return client_id_; }
  uint32_t crypt_val() const { return crypt_val_; }
  uint16_t port() const { return port_; }
  uint8_t result() const { return result_; }
  std::string ip() const { return ip_; }

  enum eResult : uint8_t {
    OK = 0,
    FAILED,
    FULL,
    INVALID_CHANNEL,
    CHANNEL_NOT_ACTIVE,
    INVALID_AGE
  };

 protected:
  void pack() { *this << result_ << client_id_ << crypt_val_ << ip_ << port_; }

 private:
  uint32_t client_id_;
  uint32_t crypt_val_;
  uint16_t port_;
  uint8_t result_;
  std::string ip_;
};

class SrvJoinServerReply : public CRosePacket {
 public:
  SrvJoinServerReply(uint8_t result, uint32_t id, uint32_t pay_flag = 0)
      : CRosePacket(ePacketType::PAKSC_JOIN_SERVER_REPLY),
        result_(result),
        id_(id),
        pay_flag_(pay_flag) {}

  virtual ~SrvJoinServerReply() {}

  uint8_t result() const { return result_; }
  uint32_t id() const { return id_; }
  uint32_t payflag() const { return pay_flag_; }

  enum eResult : uint8_t {
    OK = 0,
    FAILED,
    TIME_OUT,
    INVALID_PASSWORD,
    ALREADY_LOGGEDIN
  };

 protected:
  void pack() { *this << result_ << id_ << pay_flag_; }

 private:
   uint8_t result_;
   uint32_t id_;
   uint32_t pay_flag_;
};

//-----------------------------------------------
// ISC Packets
//-----------------------------------------------
class IscServerRegister : public CRosePacket {
 public:
   IscServerRegister( uint8_t buffer[MAX_PACKET_SIZE] ) : CRosePacket( buffer )
   {
     if (type() != ePacketType::ISC_SERVER_REGISTER)
       throw std::runtime_error( "Not the right packet!" );
     uint32_t _size = size();
     auto data = std::unique_ptr<uint8_t[]>( new uint8_t[_size] );
     memset( data.get(), 0, _size );
     for(uint32_t i = 0; i < _size; ++i)
       *this >> data[i];

     if (server_reg_.SerializeToArray(data.get(), _size) == false)
       throw std::runtime_error("Couldn't serialize the data");
   }

  IscServerRegister(const std::string &name, const std::string &ip, int32_t port, int32_t type, int32_t right)
      : CRosePacket(ePacketType::ISC_SERVER_REGISTER) {
    server_reg_.set_name( name );
    server_reg_.set_addr( ip );
    server_reg_.set_port( port );
    server_reg_.set_type( (iscPacket::ServerReg_ServerType)type );
    server_reg_.set_accright( right );
  }

  iscPacket::ServerReg server_reg() const { return server_reg_; }

 protected:
  void pack() {
    int _size = server_reg_.ByteSize();
    auto data = std::unique_ptr<uint8_t[]>( new uint8_t[_size] );
    memset( data.get(), 0, _size );
    if (server_reg_.SerializeToArray( data.get(), _size ) == false)
      throw std::runtime_error("Couldn't serialize the data");

    for(int i = 0; i < _size; ++i)
      *this << data[i];
  }

 private:
   iscPacket::ServerReg server_reg_;
};

}

#endif /* !_PACKETCLASSES_H_ */
