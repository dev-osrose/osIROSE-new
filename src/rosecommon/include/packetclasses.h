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

class CliScreenShotReq : public CRosePacket {
public:
  CliScreenShotReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> count_;
  }
  CliScreenShotReq(uint16_t count = 1)
      : CRosePacket(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ),
        count_(count) {}

  virtual ~CliScreenShotReq() {}

private:
  uint16_t count_;
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
    if (type() != ePacketType::PAKCS_JOIN_SERVER_REQ)
      throw std::runtime_error("Not the right packet!");
    char pass[32];
    *this >> channel_id_ >> pass;
    password_ = std::string(pass, 32);
  }
  CliJoinServerReq(uint32_t id, const std::string &pass)
      : CRosePacket(ePacketType::PAKCS_JOIN_SERVER_REQ),
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

class CliCreateCharReq : public CRosePacket {
 public:
  CliCreateCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_CREATE_CHAR_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> race_ >> stone_ >> hair_ >> face_ >> weapon_ >> zone_ >> name_;
  }
  CliCreateCharReq(const std::string &name, uint8_t race, uint8_t stone, uint8_t hair, uint8_t face, uint8_t weapon, uint8_t zone)
      : CRosePacket(ePacketType::PAKCS_CREATE_CHAR_REQ),
        race_(race),
        stone_(stone),
        hair_(hair),
        face_(face),
        weapon_(weapon),
        zone_(zone),
        name_(name) {}

  virtual ~CliCreateCharReq() {}

  std::string name() const { return name_; }
  uint8_t race() const { return race_; }
  uint8_t stone() const { return stone_; }
  uint8_t hair() const { return hair_; }
  uint8_t face() const { return face_; }
  uint8_t weapon() const { return weapon_; }
  uint8_t zone() const { return zone_; }

 protected:
  void pack() { *this << race_ << stone_ << hair_ << face_ << weapon_ << zone_ << name_; }

 private:
  uint8_t race_;
  uint8_t stone_;
  uint8_t hair_;
  uint8_t face_;
  uint8_t weapon_;
  uint8_t zone_;
  std::string name_;
};

class CliDeleteCharReq : public CRosePacket {
 public:
  CliDeleteCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_DELETE_CHAR_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> char_id_ >> delete_ >> name_;
  }
  CliDeleteCharReq(const std::string &name, uint8_t id, uint8_t del)
      : CRosePacket(ePacketType::PAKCS_DELETE_CHAR_REQ),
        char_id_(id),
        delete_(del),
        name_(name) {}

  virtual ~CliDeleteCharReq() {}

  uint8_t char_id() const { return char_id_; }
  bool isDelete() const { return (delete_ != 0) ? true : false; }
  std::string name() const { return name_; }

 protected:
  void pack() { *this << char_id_ << delete_; }

 private:
  uint8_t char_id_;
  uint8_t delete_;
  std::string name_;
};

class CliSelectCharReq : public CRosePacket {
 public:
  CliSelectCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_SELECT_CHAR_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> char_id_ >> run_mode_ >> ride_mode_ >> name_;
  }
  CliSelectCharReq(const std::string &name, uint8_t id, uint8_t run, uint8_t ride)
      : CRosePacket(ePacketType::PAKCS_SELECT_CHAR_REQ),
        char_id_(id),
        run_mode_(run),
        ride_mode_(ride),
        name_(name) {}

  virtual ~CliSelectCharReq() {}

  uint8_t char_id() const { return char_id_; }
  uint8_t run_mode() const { return run_mode_; }
  uint8_t ride_mode() const { return ride_mode_; }
  std::string name() const { return name_; }

 protected:
  void pack() { *this << char_id_ << run_mode_ << ride_mode_ << name_; }

 private:
  uint8_t char_id_;
  uint8_t run_mode_;
  uint8_t ride_mode_;
  std::string name_;
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

class SrvScreenShotReply : public CRosePacket {
 public:
  SrvScreenShotReply()
      : CRosePacket(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY),
        year_(2016),
        month_(4),
        day_(20),
        hour_(5),
        min_(52) {}

 protected:
  void pack() { *this << year_ << month_ << day_ << hour_ << min_; }

 private:
   uint16_t year_;
   uint8_t month_;
   uint8_t day_;
   uint8_t hour_;
   uint8_t min_;
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

class SrvCharacterListReply : public CRosePacket {
 public:
  SrvCharacterListReply()
      : CRosePacket(ePacketType::PAKCC_CHAR_LIST_REPLY),
        character_count_(0) {}

  virtual ~SrvCharacterListReply() {}

  void addCharacter(const std::string &name, uint8_t race,
                  uint16_t level, uint16_t job,
                  uint32_t delete_time = 0) {
    ++character_count_;
    char_info character(name, race, level, job, delete_time);
    character_list_.push_back( character );

    for (int idx = 0; idx < MAX_EQUIPPED_ITEMS; ++idx)
      addEquipItem(character_count_-1, idx);
  }

  void addEquipItem(uint8_t char_id, uint8_t slot, uint16_t item_id = 0,
                  uint16_t gem = 0, uint8_t socket = 0,
                  uint8_t grade = 0) {

    if(char_id < character_count_ && slot < MAX_EQUIPPED_ITEMS) {
      equip_item item = character_list_[char_id].items_[slot];
      item.id_ = item_id;
      item.gem_op_ = gem;
      item.socket_ = socket;
      item.grade_ = grade;
      character_list_[char_id].items_[slot] = item;
    }
  }

 protected:
  void pack() {
    *this << character_count_;

    for (auto &character : character_list_) {
      *this << character.race_ << character.level_ << character.job_ << character.remain_sec_unitl_delete_ << character.platinum_;

      for(int i = 0; i < MAX_EQUIPPED_ITEMS; ++i) {
        for(int j = 0; j < 4; ++j)
          *this << character.items_[i].data[j];
      }
      *this << character.name_;
    }
  }

 private:
  uint8_t character_count_;

  enum equipped_position {
    EQUIP_FACE = 0,
    EQUIP_HAIR = 1,
    EQUIP_HELMET = 2,
    EQUIP_ARMOR = 3,
    EQUIP_GAUNTLET = 4,
    EQUIP_BOOTS = 5,
    EQUIP_GOGGLES = 6,
    EQUIP_FACE_ITEM = EQUIP_GOGGLES,
    EQUIP_BACKPACK = 7,
    EQUIP_WEAPON_R = 8,
    EQUIP_WEAPON_L = 9,
    MAX_EQUIPPED_ITEMS
  };

  struct equip_item {
    union {
      PACK(struct {
        uint16_t id_ : 10; // 0~1023
        uint16_t gem_op_ : 9; // 0~512
        uint8_t socket_ : 1; // 0~1
        uint8_t grade_ : 4; // 0~15
      });
      uint8_t data[3];
    };

    equip_item(uint16_t id = 0, uint16_t gem = 0, uint8_t socket = 0, uint8_t grade = 0) :
      id_(id),
      gem_op_(gem),
      socket_(socket),
      grade_(grade) {}
  };

  struct char_info {
    uint32_t remain_sec_unitl_delete_;
    uint16_t level_;
    uint16_t job_;
    uint8_t race_;
    uint8_t platinum_;
    equip_item items_[MAX_EQUIPPED_ITEMS];
    std::string name_;

    char_info(const std::string &name, uint8_t race = 0, uint16_t level = 0,
         uint16_t job = 0, uint32_t delete_time = 0, uint8_t platinum = 0)
        : remain_sec_unitl_delete_(delete_time),
          level_(level),
          job_(job),
          race_(race),
          platinum_(platinum),
          name_(name) {}
  };
  std::vector<char_info> character_list_;
};

class SrvDeleteCharReply : public CRosePacket {
 public:
  SrvDeleteCharReply(const std::string &name, uint32_t remaining_time)
      : CRosePacket(ePacketType::PAKCC_DELETE_CHAR_REPLY),
        remaining_time_(remaining_time),
        name_(name) {}

  virtual ~SrvDeleteCharReply() {}

  uint32_t remaining_time() const { return remaining_time_; }
  std::string name() const { return name_; }

 protected:
  void pack() { *this << remaining_time_ << name_; }

 private:
   uint32_t remaining_time_;
   std::string name_;
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
