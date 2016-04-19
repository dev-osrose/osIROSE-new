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

class CRosePacket {
	public:
		CRosePacket(ePacketType type, uint8_t CRC = 0, uint8_t reserved = 0) : current_(buffer_), size_(0), type_(type), CRC_(CRC) {
			*this << size_ << type_ << reserved << CRC;
		}

		CRosePacket(uint16_t type, uint8_t CRC = 0, uint8_t reserved = 0) : current_(buffer_), size_(0), type_(static_cast<ePacketType>(type)), CRC_(CRC) {
			*this << size_ << type_ << reserved << CRC;
		}

		CRosePacket(const uint8_t buffer[MAX_PACKET_SIZE]) : current_(buffer_) {
			std::memcpy(buffer_, buffer, sizeof(buffer_));
			uint8_t tmp;
			*this >> size_ >> type_ >> tmp >> CRC_;
		}
		virtual ~CRosePacket() {}

		ePacketType type() const {return type_;}
		uint16_t size() const {return size_;}
		uint8_t CRC() const {return CRC_;}
		uint8_t* data() {return buffer_+6;}
		void resetCurrent() {current_ = buffer_;}

		std::unique_ptr<uint8_t[]> getPacked() {
			pack();
			auto res = std::unique_ptr<uint8_t[]>(new uint8_t[size_]);
			std::memcpy(res.get(), buffer_, size_);
			return res;
		}

		static uint16_t size(const uint8_t buffer[MAX_PACKET_SIZE]) {
			return read<uint16_t>(const_cast<uint8_t*>(buffer));
		}

		static ePacketType type(const uint8_t buffer[MAX_PACKET_SIZE]) {
			return read<ePacketType>(const_cast<uint8_t*>(buffer + sizeof(uint16_t)));
		}

		static uint8_t CRC(const uint8_t buffer[MAX_PACKET_SIZE]) {
			return read<uint8_t>(const_cast<uint8_t*>(buffer +
						sizeof(uint16_t) + sizeof(ePacketType) + sizeof(uint8_t)));
		}

//		static std::unique_ptr<uint8_t[]> data(const uint8_t buffer[MAX_PACKET_SIZE]) {
//                        auto res = std::unique_ptr<uint8_t[]>(new uint8_t[size(buffer)]);
//                         std::memcpy(res.get(), buffer + 6, size(buffer));
//			return res;
//		}

	protected:
		virtual void pack() {}

		template <typename T, typename std::enable_if<!is_container<T>::value>::type* = nullptr>
		friend CRosePacket &operator<<(CRosePacket &os, const T &data) {
			os.writeNext<T>(data);
			return os;
		}

		template <typename T>
		friend typename std::enable_if<is_container<T>::value, CRosePacket>::type &operator<<(CRosePacket &os, const T &data) {
			for (const auto &it : data)
				os.writeNext<typename T::value_type>(it);
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
				os.writeNext<char>(*(data++));
			return os;
		}

		template <size_t count>
		friend CRosePacket &operator<<(CRosePacket &os, const char (&data)[count]) {
			for (size_t i = 0; i < count; ++i)
				os.writeNext<char>(data[i]);
			return os;
		}

		template <typename T, typename std::enable_if<!is_container<T>::value>::type* = nullptr>
		friend CRosePacket &operator>>(CRosePacket &os, T &data) {
			data = os.readNext<T>();
			return os;
		}

		template <typename T>
		friend typename std::enable_if<is_container<T>::value, CRosePacket>::type &operator>>(CRosePacket &os, T &data) {
			for (auto &it : data)
				it = os.readNext<typename T::value_type>();
			return os;
		}

		template <size_t count>
		friend CRosePacket &operator>>(CRosePacket &os, char (&data)[count]) {
			for (size_t i = 0; i < count; ++i)
				data[i] = os.readNext<char>();
			return os;
		}

		friend CRosePacket &operator>>(CRosePacket &os, std::string &data) {
			do {
				data.push_back(os.readNext<char>());
			} while (data.back());
			return os;
		}

	private:
		template <typename T, typename = std::enable_if<!is_container<T>::value>>
		static T read(uint8_t *data) {
			static_assert(std::is_copy_constructible<T>::value, "CRosePacket doesn't know how to copy construct this type!");
			return *reinterpret_cast<T*>(data);
		}

		template <typename T, typename = std::enable_if<!is_container<T>::value>>
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
		uint8_t CRC_;
};

}

#endif /* !_CROSEPACKET_H_ */
