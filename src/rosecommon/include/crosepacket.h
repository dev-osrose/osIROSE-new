/*!
 *  \file crosepacket.h
 *
 *  This file contains CRosePacket
 *
 */
#ifndef __CROSEPACKET_H__
#define __CROSEPACKET_H__

#include <array>
#include <cstring>
#include <type_traits>
#include <assert.h>
#include "epackettype.h"

template <typename T>
struct HasConstIterator {
private:
  template <typename C> static char test(typename C::const_iterator*);
  template <typename C> static int test(...);
public:
  enum {value = sizeof(test<T>(0)) == sizeof char};
};

class CRosePacket {
public:
  CRosePacket(const std::array<uint8_t, MAX_PACKET_SIZE> &data) : payload_(data),
              current_(payload_.begin()), size_(readNext<uint16_t>()),
              type_(readNext<ePacketType>()) {
                current_ += sizeof uint16_t; // uint8_t reserved, uint8_t CRC
  }

  CRosePacket(ePacketType type) : current_(payload_.begin()), type_(type), size_(0) {}

  ePacketType getType() const { return type_; }
  uint16_t    getSize() const { return size_; }

  std::unique_ptr<uint8_t> getPacked() const {
    return std::unique_ptr<uint8_t[]>(new uint8_t[size_]);
  }

  template <typename T>
  CRosePacket &operator<<(const T &data) {
    static_assert(std::is_trivially_copyable<T>::value ||
                  HasConstIterator<T>::value &&
                  "CRosePacket doesn't know how to deal with this type yet !");
    if (std::is_trivially_copyable<T>::value)
      writeNext<T>(data);
    else
      for (const auto &it : data)
        writeNext<T>(it);
    return *this;
  }

  template <typename T>
  CRosePacket &operator>>(T &data) {
    static_assert(std::is_trivially_copyable<T>::value &&
                  "CRosePacket doesn't know how to deal with this type yet !");
    readNext<T>(data);
    return *this;
  }

private:
  template <typename T>
  T cast(uint8_t *data) {
    return *reinterpret_cast<T*>(data);
  }

  template <typename T>
  T readNext() {
    auto tmp = current_;
    current_ += sizeof T;
      if (current_ >= payload_.end())
        return T();
    return cast<T>(tmp);
  }

  template <typename T>
  void cast(uint8_t *data, const T &data) {
    T *tmp = reinterpret_cast<T*>(data);
    *tmp = data;
  }

  template <typename T>
  void writeNext(const T &data) {
    auto tmp = current_;
    current_ += sizeof T;
    if (current_ >= payload_.end())
      return;
    cast<T>(tmp, data);
  }
  
  std::array<uint8_t, MAX_PACKET_SIZE> payload_;
  std::array<uint8_t, MAX_PACKET_SIZE>::iterator current_;
  uint16_t size_;
  ePacketType type_;
};

#endif
