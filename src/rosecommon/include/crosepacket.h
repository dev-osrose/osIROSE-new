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
 * \file crosepacket.h
 *
 * \author L3nn0x
 * \date april 2016
 *
 * The packet base class to communicate with the client
 *
 */
#ifndef _CROSEPACKET_H_
#define _CROSEPACKET_H_

#include <memory>
#include <cstring>
#include <cassert>
#include <type_traits>
#include "epackettype.h"
#include "iscontainer.h"

#define MAX_PACKET_SIZE 0x7FF

namespace RoseCommon {

/*!
 * \class CRosePacket
 *
 * \brief The base class for the packet structure and logic.
 *
 * This class is used to encapsulate the basic logic of the packets and their encoding.
 * You should not use this class directly but create a subclass which will implement the correct logic for the packet.
 * Subclasses can use >> and << to write data to the underlying buffer. Both operators support assignable and copyable types, std::string, char[]
 * and all objects that can be iterated over.
 * /!\ If you send a std::string to operator<< then the string WILL be null terminated. If you don't want this behavior send a char* or char[]
 * /!\ If you send a std::string to operator>> then the packet will fill it until it finds a '\0', greedy. If you don't want this behavior send a char[]
 *
 * \author L3nn0x
 * \date april 2016
 */
class CRosePacket {
    public:
        CRosePacket(ePacketType type, uint16_t CRC = 0, uint8_t reserved = 0) : current_(buffer_), size_(0), type_(type), CRC_(CRC) {
          (void)reserved;
          *this << size_ << (uint16_t)type_ << CRC;
        }

        CRosePacket(uint16_t type, uint16_t CRC = 0, uint8_t reserved = 0) : current_(buffer_), size_(0), type_(static_cast<ePacketType>(type)), CRC_(CRC) {
          (void)reserved;
          *this << size_ << (uint16_t)type_ << CRC;
        }

        CRosePacket(const uint8_t buffer[MAX_PACKET_SIZE]) : current_(buffer_) {
          std::memcpy(buffer_, buffer, size(buffer_));
          *this >> size_ >> type_ >> CRC_;
        }
        virtual ~CRosePacket() {}

        ePacketType type() const {return type_;}
        uint16_t size() const {return size_;}
        uint16_t CRC() const {return CRC_;}
        uint8_t* data() {return buffer_+6;}
        void resetCurrent() {current_ = buffer_;}

        /*!
         * \brief Function to get the structured buffer
         *
         * This function will call pack(), and return a copy of it's internal buffer
         */
        std::unique_ptr<uint8_t[]> getPacked() {
            pack();
            auto res = std::unique_ptr<uint8_t[]>(new uint8_t[size_]);
            std::memcpy(res.get(), buffer_, size_);
            return res;
        }

        /*!
         * \brief Helper function to extract the size from a buffer
         */
        static uint16_t size(const uint8_t buffer[MAX_PACKET_SIZE]) {
            return read<uint16_t>(const_cast<uint8_t*>(buffer));
        }

        /*!
         * \brief Helper function to extract the type from a buffer
         */
        static ePacketType type(const uint8_t buffer[MAX_PACKET_SIZE]) {
            return read<ePacketType>(const_cast<uint8_t*>(buffer + sizeof(uint16_t)));
        }

        /*!
         * \brief Helper function to extract the CRC code from a buffer
         */
        static uint8_t CRC(const uint8_t buffer[MAX_PACKET_SIZE]) {
            return read<uint8_t>(const_cast<uint8_t*>(buffer +
                                sizeof(uint16_t) + sizeof(ePacketType)));
        }

    protected:
        /*!
         * \brief Reimplement this function to include last minute logic before the packet is written to an external buffer
         */
        virtual void pack() {}

        template <typename T, typename std::enable_if<!Core::is_container<T>::value>::type* = nullptr>
        friend CRosePacket &operator<<(CRosePacket &os, const T &data) {
            os.writeNext<T>(data);
            return os;
        }

        template <typename T>
        friend typename std::enable_if<Core::is_container<T>::value, CRosePacket>::type &operator<<(CRosePacket &os, const T &data) {
            for (const auto &it : data)
                os << it;
            return os;
        }

        friend CRosePacket &operator<<(CRosePacket &os, const std::string &data) {
            for (const auto &it : data)
                os.writeNext<char>(it);
            os.writeNext<char>(0);
            return os;
        }

        friend CRosePacket &operator<<(CRosePacket &os, const char *data) {
            while (*data)
                os << *(data++);
            return os;
        }

        template <size_t count>
        friend CRosePacket &operator<<(CRosePacket &os, const char (&data)[count]) {
            for (size_t i = 0; i < count; ++i)
                os << data[i];
            return os;
        }

        template <typename T, typename std::enable_if<!Core::is_container<T>::value>::type* = nullptr>
        friend CRosePacket &operator>>(CRosePacket &os, T &data) {
            data = os.readNext<T>();
            return os;
        }

        template <typename T>
        friend typename std::enable_if<Core::is_container<T>::value, CRosePacket>::type &operator>>(CRosePacket &os, T &data) {
            for (auto &it : data)
              os >> it;
            return os;
        }

        template <size_t count>
        friend CRosePacket &operator>>(CRosePacket &os, char (&data)[count]) {
            for (size_t i = 0; i < count; ++i)
              os >> data[i];
            return os;
        }

        friend CRosePacket &operator>>(CRosePacket &os, std::string &data) {
            while (auto next = os.readNext<char>())
                data.push_back(next);
            return os;
        }

    private:
        template <typename T, typename = std::enable_if<!Core::is_container<T>::value>>
        static T read(uint8_t *data) {
            static_assert(std::is_copy_constructible<T>::value, "CRosePacket doesn't know how to copy construct this type!");
            return *reinterpret_cast<T*>(data);
        }

        template <typename T, typename = std::enable_if<!Core::is_container<T>::value>>
        static void write(uint8_t *data, const T &payload) {
            static_assert(std::is_copy_assignable<T>::value, "CRosePacket doesn't know how to copy assign this type!");
            *reinterpret_cast<T*>(data) = payload;
        }

        template <typename T>
        T readNext() {
            if (current_ + sizeof(T) >= buffer_ + sizeof(buffer_))
                return T();
            auto tmp = current_;
            current_ += sizeof(T);
            return read<T>(tmp);
        }

        template <typename T>
        void writeNext(const T &payload) {
            if (current_ + sizeof(T) >= buffer_ + sizeof(buffer_))
                return;
            auto tmp = current_;
            current_ += sizeof(T);
            size_ += sizeof(T);
            write<T>(tmp, payload);
            write<uint16_t>(buffer_, size_);
        }

        uint8_t buffer_[MAX_PACKET_SIZE];
        uint8_t *current_;
        uint16_t size_;
        ePacketType type_;
        uint16_t CRC_;
};

}

#endif /* !_CROSEPACKET_H_ */
