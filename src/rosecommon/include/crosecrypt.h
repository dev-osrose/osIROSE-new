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

#ifndef __CROSECRYPT__
#define __CROSECRYPT__

#include <string.h>
#include <stdint.h>
// Pulled from Brett19's cRose Server
// https://github.com/brett19/cROSE

//* Official CrcTable
///*
//
namespace RoseCommon {

#ifdef __GNUC__
#define PACK(...) __VA_ARGS__ __attribute__((__packed__))
#else
#define PACK(...) __pragma(pack(push, 1)) __VA_ARGS__ __pragma(pack(pop))
#endif

const unsigned char CrcTable[] =
    "\x00\x5E\xBC\xE2\x61\x3F\xDD\x83\xC2\x9C\x7E\x20\xA3\xFD\x1F\x41"
    "\x9D\xC3\x21\x7F\xFC\xA2\x40\x1E\x5F\x01\xE3\xBD\x3E\x60\x82\xDC"
    "\x23\x7D\x9F\xC1\x42\x1C\xFE\xA0\xE1\xBF\x5D\x03\x80\xDE\x3C\x62"
    "\xBE\xE0\x02\x5C\xDF\x81\x63\x3D\x7C\x22\xC0\x9E\x1D\x43\xA1\xFF"
    "\x46\x18\xFA\xA4\x27\x79\x9B\xC5\x84\xDA\x38\x66\xE5\xBB\x59\x07"
    "\xDB\x85\x67\x39\xBA\xE4\x06\x58\x19\x47\xA5\xFB\x78\x26\xC4\x9A"
    "\x65\x3B\xD9\x87\x04\x5A\xB8\xE6\xA7\xF9\x1B\x45\xC6\x98\x7A\x24"
    "\xF8\xA6\x44\x1A\x99\xC7\x25\x7B\x3A\x64\x86\xD8\x5B\x05\xE7\xB9"
    "\x8C\xD2\x30\x6E\xED\xB3\x51\x0F\x4E\x10\xF2\xAC\x2F\x71\x93\xCD"
    "\x11\x4F\xAD\xF3\x70\x2E\xCC\x92\xD3\x8D\x6F\x31\xB2\xEC\x0E\x50"
    "\xAF\xF1\x13\x4D\xCE\x90\x72\x2C\x6D\x33\xD1\x8F\x0C\x52\xB0\xEE"
    "\x32\x6C\x8E\xD0\x53\x0D\xEF\xB1\xF0\xAE\x4C\x12\x91\xCF\x2D\x73"
    "\xCA\x94\x76\x28\xAB\xF5\x17\x49\x08\x56\xB4\xEA\x69\x37\xD5\x8B"
    "\x57\x09\xEB\xB5\x36\x68\x8A\xD4\x95\xCB\x29\x77\xF4\xAA\x48\x16"
    "\xE9\xB7\x55\x0B\x88\xD6\x34\x6A\x2B\x75\x97\xC9\x4A\x14\xF6\xA8"
    "\x74\x2A\xC8\x96\x15\x4B\xA9\xF7\xB6\xE8\x0A\x54\xD7\x89\x6B\x35";
//*/

class RoseRandomNumber {
 public:
  RoseRandomNumber(unsigned int seed);

  enum Type {
    BC,
    AC,
    MY,
    VC,
  };

  int get();
  void setType(Type type);

 private:
  int R_BC();
  int R_AC();
  int R_MY();
  int R_VC();

  RoseRandomNumber::Type m_type;
  int m_VcSeed;
  int m_BcSeed;
  int m_AcSeed;
  int m_MySeed;
};

// PACK(
struct HeadCryptedServer {
  uint32_t AddBufferLen2 : 3;     // S2
  uint32_t AddTableValue1 : 3;    // R1
  uint32_t Command3 : 3;          // T3
  uint32_t EncryptValue1 : 3;     // R7
  uint32_t AddBufferLen3 : 3;     // S3
  uint32_t AddTableValue3 : 3;    // R3
  uint32_t Command2 : 3;          // T2
  uint32_t AddTableValue4 : 2;    // R4
  uint32_t Command1 : 3;          // T1
  uint32_t EncryptAddValue1 : 2;  // R5
  uint32_t AddBufferLen4 : 2;     // S4
  uint32_t EncryptAddValue2 : 2;  // R6
  uint8_t AddBufferLen1 : 3;      // S1
  uint8_t AddTableValue2 : 3;     // R2
  uint8_t Command4 : 2;           // T4
};                                // );

// PACK(
struct HeadCryptedClient {
  uint32_t Command2 : 3;          // T2
  uint32_t AddTableValue2 : 3;    // R2
  uint32_t AddBufferLen1 : 3;     // S1
  uint32_t AddTableValue3 : 3;    // R3
  uint32_t Command1 : 3;          // T1
  uint32_t EncryptValue1 : 3;     // R7
  uint32_t AddBufferLen2 : 3;     // S2
  uint32_t EncryptAddValue2 : 2;  // R6
  uint32_t AddBufferLen3 : 3;     // S3
  uint32_t AddTableValue4 : 2;    // R4
  uint32_t Command4 : 2;          // T4
  uint32_t EncryptAddValue1 : 2;  // R5
  uint8_t Command3 : 3;           // T3
  uint8_t AddTableValue1 : 3;     // R1
  uint8_t AddBufferLen4 : 2;      // S4
};                                // );

// PACK(
struct HeadDecrypted {
  uint64_t AddBufferLen1 : 3;
  uint64_t AddBufferLen2 : 3;
  uint64_t AddBufferLen3 : 3;
  uint64_t AddBufferLen4 : 2;
  uint64_t Command1 : 3;
  uint64_t Command2 : 3;
  uint64_t Command3 : 3;
  uint64_t Command4 : 2;
  uint64_t AddTableValue1 : 3;
  uint64_t AddTableValue2 : 3;
  uint64_t AddTableValue3 : 3;
  uint64_t AddTableValue4 : 2;
  uint64_t EncryptAddValue1 : 2;
  uint64_t EncryptAddValue2 : 2;
  uint64_t EncryptValue1 : 3;
};  //);

// PACK(
struct Head {
  uint64_t AddBufferLen : 11;
  uint64_t Command : 11;
  uint64_t AddTableValue : 11;
  uint64_t EncryptAddValue : 4;
  uint64_t EncryptValue : 3;
};  //);

template <typename T, typename U>
inline void FlipHeadMain(T a, U b) {
  a->AddBufferLen1 = b->AddBufferLen1;
  a->AddBufferLen2 = b->AddBufferLen2;
  a->AddBufferLen3 = b->AddBufferLen3;
  a->AddBufferLen4 = b->AddBufferLen4;
  a->Command1 = b->Command1;
  a->Command2 = b->Command2;
  a->Command3 = b->Command3;
  a->Command4 = b->Command4;
  a->EncryptValue1 = b->EncryptValue1;
  a->EncryptAddValue1 = b->EncryptAddValue1;
  a->EncryptAddValue2 = b->EncryptAddValue2;
}

template <typename T, typename U>
inline void FlipHeadFinal(T a, U b) {
  a->AddTableValue1 = b->AddTableValue1;
  a->AddTableValue2 = b->AddTableValue2;
  a->AddTableValue3 = b->AddTableValue3;
  a->AddTableValue4 = b->AddTableValue4;
}

class PacketCodec {
 public:
  PacketCodec(unsigned int seed = 0x0042D266);
  ~PacketCodec();

  void encodeServerPacket(unsigned char* buffer);
  uint16_t decodeClientHeader(unsigned char* buffer);
  bool decodeClientBody(unsigned char* buffer);

  void encodeClientPacket(unsigned char* buffer);
  uint16_t decodeServerHeader(unsigned char* buffer);
  bool decodeServerBody(unsigned char* buffer);

  void changeSeed(unsigned int seed);

 private:
  bool find_DupTABLE(int val, int table, int range);
  bool find_DupINDEX(int val, int range);

  RoseRandomNumber m_Cr;
  int** m_Rt;
  unsigned short* m_Ri;
};

}

#endif
