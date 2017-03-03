#pragma once

#include <string>

struct BasicInfo {
  enum Cmd {
    STOP = 0,
    MOVE = 1,
    ATTACK = 2,
    DIE = 3,
    PICKUP = 4,
    SKILL2SELF = 6,
    SKILL2OBJ = 7,
    SKILL2POS = 8,
    RUNAWAY = 0x8009,
    SIT = 10
  };

  BasicInfo() : level_( 0 ),
                xp_( 0 ),
                id_( 0 ),
                tag_( 0 ),
                teamId_( 0 ),
                targetId_( 0 ),
                command_( STOP ),
                loggedIn_( false ) {}

  std::string name_;
  uint16_t level_;
  uint32_t xp_;
  uint16_t id_;
  uint32_t tag_;
  int32_t teamId_;
  uint16_t targetId_;
  uint16_t command_;
  bool loggedIn_;
};
