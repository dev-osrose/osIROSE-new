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

#include <cstdlib>
#include "crosecrypt.h"

namespace RoseCommon {

RoseRandomNumber::RoseRandomNumber(unsigned int seed)
    : m_type(RoseRandomNumber::BC),
      m_VcSeed(seed),
      m_BcSeed(seed),
      m_AcSeed(seed),
      m_MySeed(seed) {}

void RoseRandomNumber::setType(RoseRandomNumber::Type type) { m_type = type; }

int RoseRandomNumber::R_BC() {
  m_BcSeed = 0x8088405 * m_BcSeed + 1;
  return m_BcSeed / 0x10000;
}

int RoseRandomNumber::R_AC() {
  m_AcSeed = (0x41C64E6D * m_AcSeed + 12345) & 0x7FFFFFFF;
  return m_AcSeed;
}

int RoseRandomNumber::R_MY() {
  m_MySeed = 0x41C64E6D * m_MySeed + 12345;
  return m_MySeed / 0x10000;
}

int RoseRandomNumber::R_VC() {
  m_VcSeed = (0x343FD * m_VcSeed + 0x269EC3) & 0x7FFFFFFF;
  return m_VcSeed / 0x10000;
}

int RoseRandomNumber::get() {
  if (m_type == RoseRandomNumber::BC)
    return R_BC();
  else if (m_type == RoseRandomNumber::AC)
    return R_AC();
  else if (m_type == RoseRandomNumber::MY)
    return R_MY();
  else
    return R_VC();
}

PacketCodec::PacketCodec(unsigned int seed) : m_Cr(0) {
  m_Rt = new int* [0x10];
  for (int i = 0; i < 0x10; i++) m_Rt[i] = new int[0x800];
  m_Ri = new unsigned short[0x200];

  changeSeed(seed);
}

void PacketCodec::changeSeed(unsigned int seed) {
  m_Cr = RoseRandomNumber(seed);
  for (int i = 0; i < 0x10; i++) {
    m_Cr.setType(static_cast<RoseRandomNumber::Type>(i & 3));
    for (int j = 0; j < 0x800; j++) {
      do {
        m_Rt[i][j] = m_Cr.get();
      } while (find_DupTABLE(m_Rt[i][j], 0, j));
    }
  }

  m_Cr.setType(RoseRandomNumber::AC);
  for (int i = 0; i < 0x200; i++) {
    do {
      m_Ri[i] = m_Cr.get() & 0x7FF;
    } while (find_DupINDEX(m_Ri[i], i));
  }
}

PacketCodec::~PacketCodec() {
  for (int i = 0; i < 0x10; i++) delete[] m_Rt[i];
  delete[] m_Rt;
  delete[] m_Ri;
}

void PacketCodec::encodeServerPacket(unsigned char* buffer) {
  Head head;
  memset(&head, 0, sizeof(head));
  head.AddTableValue = 0x100;  // rand() % 0x1FF + 1;
  head.EncryptAddValue = 0x04;  // rand() % 0xF + 1;
  head.EncryptValue = head.AddTableValue + head.EncryptAddValue;
  head.AddBufferLen = *((unsigned short*)&buffer[0]);
  head.Command = *((unsigned short*)&buffer[2]);
  FlipHeadMain((HeadCryptedServer*)buffer, (HeadDecrypted*)&head);

  unsigned char Checksum = 0;
  for (int i = 0; i < 5; i++) {
    Checksum = CrcTable[((unsigned char*)&head)[i] ^ Checksum];
    buffer[i] ^= m_Rt[i][head.AddTableValue];
  }
  for (int i = 6; i < head.AddBufferLen; i++) {
    Checksum = CrcTable[buffer[i] ^ Checksum];
    buffer[i] ^= m_Rt[(head.EncryptAddValue + i) &
                      0xF][(head.AddTableValue + i) & 0x7FF];
  }
  buffer[5] = Checksum;

  FlipHeadFinal((HeadCryptedServer*)buffer, (HeadDecrypted*)&head);
}

void PacketCodec::encodeClientPacket(unsigned char* buffer) {
  Head head;
  memset(&head, 0, sizeof(head));
  head.AddTableValue = 0x100;  // rand() % 0x1FF + 1
  head.EncryptAddValue = 0x04;
  head.EncryptValue = 0;  // head.AddTableValue + head.EncryptAddValue;
  head.AddBufferLen = *((unsigned short*)&buffer[0]);
  head.Command = *((unsigned short*)&buffer[2]);
  FlipHeadMain((HeadCryptedClient*)buffer, (HeadDecrypted*)&head);

  unsigned char Checksum = 0;
  for (int i = 0; i < 5; i++) {
    Checksum = CrcTable[((unsigned char*)&head)[i] ^ Checksum];
    buffer[i] ^= m_Rt[i][head.AddTableValue];
  }
  for (int i = 6; i < head.AddBufferLen; i++) {
    Checksum = CrcTable[buffer[i] ^ Checksum];
    buffer[i] ^= m_Rt[(head.EncryptAddValue + i) &
                      0xF][(head.AddTableValue + i) & 0x7FF];
  }
  buffer[5] = Checksum;

  FlipHeadFinal((HeadCryptedClient*)buffer, (HeadDecrypted*)&head);
}

uint16_t PacketCodec::decodeServerHeader(unsigned char* buffer) {
  Head head;
  memset(&head, 0, sizeof(head));
  FlipHeadFinal((HeadDecrypted*)&head, (HeadCryptedServer*)buffer);

  for (int i = 0; i < 5; i++) {
    buffer[i] ^= m_Rt[i][head.AddTableValue];
  }

  FlipHeadMain((HeadDecrypted*)&head, (HeadCryptedServer*)buffer);
  memcpy(buffer, &head, 5);
  return head.AddBufferLen;
}

uint16_t PacketCodec::decodeClientHeader(unsigned char* buffer) {
  Head head;
  memset(&head, 0, sizeof(head));
  FlipHeadFinal((HeadDecrypted*)&head, (HeadCryptedClient*)buffer);

  for (int i = 0; i < 5; i++) {
    buffer[i] ^= m_Rt[i][head.AddTableValue];
  }

  FlipHeadMain((HeadDecrypted*)&head, (HeadCryptedClient*)buffer);
  memcpy(buffer, &head, 5);
  return head.AddBufferLen;
}

bool PacketCodec::decodeServerBody(unsigned char* buffer) {
  Head head;
  memset(&head, 0, sizeof(head));
  memcpy(&head, buffer, 6);
  unsigned short buflen = (unsigned short)(head.AddBufferLen);

  unsigned char Checksum = 0;
  for (int i = 0; i < 5; i++) {
    Checksum = CrcTable[((unsigned char*)&head)[i] ^ Checksum];
  }
  for (int i = 6; i < buflen; i++) {
    buffer[i] ^= m_Rt[(head.EncryptAddValue + i) &
                      0xF][(head.AddTableValue + i) & 0x7FF];
    Checksum = CrcTable[buffer[i] ^ Checksum];
  }

  if (Checksum != buffer[5]) return false;

  *((unsigned short*)&buffer[0]) = buflen;
  *((unsigned short*)&buffer[2]) = head.Command;

  return true;
}

bool PacketCodec::decodeClientBody(unsigned char* buffer) {
  Head head;
  memset(&head, 0, sizeof(head));
  memcpy(&head, buffer, 6);
  unsigned short buflen =
      (unsigned short)(head.AddBufferLen - head.EncryptValue);

  unsigned char Checksum = 0;
  for (int i = 0; i < 5; i++) {
    Checksum = CrcTable[((unsigned char*)&head)[i] ^ Checksum];
  }
  for (int i = 6; i < buflen; i++) {
    buffer[i] ^= m_Rt[(head.EncryptAddValue + i) &
                      0xF][(head.AddTableValue + i) & 0x7FF];
    Checksum = CrcTable[buffer[i] ^ Checksum];
  }

  if (Checksum != buffer[5]) return false;

  *((unsigned short*)&buffer[0]) = buflen;
  *((unsigned short*)&buffer[2]) = head.Command;

  return true;
}

bool PacketCodec::find_DupTABLE(int val, int table, int range) {
  for (int i = 0; i < table; i++) {
    for (int j = 0; j < 0x7FF; j++) {
      if (m_Rt[i][j] == val) return true;
    }
  }
  for (int i = 0; i < range; i++) {
    if (m_Rt[table][i] == val) return true;
  }
  return false;
}

bool PacketCodec::find_DupINDEX(int val, int range) {
  for (int i = 0; i < range; i++) {
    if (m_Ri[i] == val) return true;
  }
  return false;
}

}
