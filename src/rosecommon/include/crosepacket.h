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
#include <vector>
#include <type_traits>
#include "epackettype.h"
#include "crosewriter.h"

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
        CRosePacket(ePacketType type, uint16_t CRC = 0, uint8_t reserved = 0) : size_(0), type_(type), CRC_(CRC) {
          (void)reserved;
        }

        CRosePacket(uint16_t type, uint16_t CRC = 0, uint8_t reserved = 0) : size_(0), type_(static_cast<ePacketType>(type)), CRC_(CRC) {
          (void)reserved;
        }

        CRosePacket(CRoseReader& reader) : size_(0), type_(static_cast<ePacketType>(0)), CRC_(0) {
            reader.get_uint16_t(size_);
            uint16_t type;
            if(reader.get_uint16_t(type) == true)
              type_ = static_cast<ePacketType>(type);
            reader.get_uint16_t(CRC_);
        }

        virtual ~CRosePacket() = default;

        ePacketType get_type() const {return type_;}

        uint16_t get_size() const {
            if (size_ != 0)
                return size_;
            CRoseSizer sizer(0, 0);
            pack(sizer);
            uint16_t size = sizer.get_size();
            size += sizeof(size_);
            size += sizeof(type_);
            size += sizeof(CRC_);
            const_cast<CRosePacket*>(this)->size_ = size;
            return size_;
        }

        uint16_t get_CRC() const {return CRC_;}

        /*!
         * \brief Function to get the structured buffer
         *
         * This function will call pack(), and return a copy of it's internal buffer
         */
        virtual std::unique_ptr<uint8_t[]> getPacked() const {
            const uint16_t size = get_size();
            auto res = std::unique_ptr<uint8_t[]>(new uint8_t[size]);
            CRoseWriter writer(res.get(), size);
            writer.set_uint16_t(size);
            writer.set_uint16_t(to_underlying(get_type()));
            writer.set_uint16_t(get_CRC());
            pack(writer); // TODO: what to do with half-written packets
            return res;
        }

        bool write_to_vector(std::vector<uint8_t>& vec) const {
            const uint16_t size = get_size();
            auto tmp = std::unique_ptr<uint8_t[]>(new uint8_t[size]);
            CRoseWriter writer(tmp.get(), size);
            writer.set_uint16_t(size);
            writer.set_uint16_t(to_underlying(get_type()));
            writer.set_uint16_t(get_CRC());
            const bool res = pack(writer);
            vec = {tmp.get(), tmp.get() + size};
            return res;
        }

        /*!
         * \brief Helper function to extract the size from a buffer
         */
        static uint16_t size(const uint8_t buffer[MAX_PACKET_SIZE]) {
            return CRoseReader::read_at<uint16_t>(buffer);
        }

        /*!
         * \brief Helper function to extract the type from a buffer
         */
        static ePacketType type(const uint8_t buffer[MAX_PACKET_SIZE]) {
            return CRoseReader::read_at<ePacketType>(buffer + sizeof(uint16_t));
        }

        /*!
         * \brief Helper function to extract the CRC code from a buffer
         */
        static uint8_t CRC(const uint8_t buffer[MAX_PACKET_SIZE]) {
            return CRoseReader::read_at<uint8_t>(buffer +
                                sizeof(uint16_t) + sizeof(ePacketType));
        }

    protected:
        virtual bool pack(CRoseBasePolicy& writer) const = 0;
    
    private:
        uint16_t size_;
        ePacketType type_;
        uint16_t CRC_;
};

}

#endif /* !_CROSEPACKET_H_ */
