#pragma once

#include "iserialize.h"

namespace RoseCommon {

class CRoseBasePolicy {
   public:
       virtual bool set_uint8_t(uint8_t data) = 0;
       virtual bool set_int8_t(int8_t data) = 0;
       virtual bool set_uint16_t(uint16_t data) = 0;
       virtual bool set_int16_t(int16_t data) = 0;
       virtual bool set_uint32_t(uint32_t data) = 0;
       virtual bool set_int32_t(int32_t data) = 0;
       virtual bool set_uint64_t(uint64_t data) = 0;
       virtual bool set_int64_t(int64_t data) = 0;
       virtual bool set_string(const std::string& data) = 0;
       virtual bool set_string(const std::string& data, size_t size) = 0;
       virtual bool set_float(float data) = 0;
       virtual bool set_double(double data) = 0;
       virtual bool set_char(char data) = 0;
       virtual bool set_iserialize(const ISerialize& data) = 0;

       virtual uint16_t get_size() const = 0;
};

template <typename Policy>
class CRosePolicy : public CRoseBasePolicy {
    public:
        CRosePolicy(uint8_t *buffer, uint16_t size) : m_current(buffer), m_buffer(buffer), m_size(size) {}

       bool set_uint8_t(uint8_t data) { return write(data); }
       bool set_int8_t(int8_t data) { return write(data); }
       bool set_uint16_t(uint16_t data) { return write(data); }
       bool set_int16_t(int16_t data) { return write(data); }
       bool set_uint32_t(uint32_t data) { return write(data); }
       bool set_int32_t(int32_t data) { return write(data); }
       bool set_uint64_t(uint64_t data) { return write(data); }
       bool set_int64_t(int64_t data) { return write(data); }
       bool set_string(const std::string& data) {
           for (const char c : data)
               if (!write(c)) return false;
           return write('\0');
       }
       bool set_string(const std::string& data, size_t size) {
           for (size_t i = 0; i < size && i < data.size(); ++i) 
               if (!write(data[i])) return false;
           return true;
       }
       bool set_float(float data) { return write(data); }
       bool set_double(double data) { return write(data); }
       bool set_char(char data) { return write(data); }
       bool set_iserialize(const ISerialize& data) { return data.write(*this); }

       uint16_t get_size() const { return m_size; }

    private:
        uint8_t *m_current, *m_buffer;
        uint16_t m_size;

        template <typename T>
        bool write(const T& data) {
            static_assert(std::is_copy_assignable_v<T>, "CRoseWriter doesn't know how to copy assign this type!");
            return Policy::template write(&m_current, m_buffer, &m_size, data);
        }
};

struct WritePolicy {
    template <typename T>
    static bool write(uint8_t** m_current, uint8_t* m_buffer, uint16_t* m_size, const T& data) {
        if (!*m_current || *m_current + sizeof(T) > m_buffer + *m_size) return false;
        *reinterpret_cast<T*>(*m_current) = data;
        *m_current += sizeof(T);
        return true;
    }
};

struct SizePolicy {
    template <typename T>
    static bool write(uint8_t**, uint8_t*, uint16_t* m_size, const T&) {
        *m_size += sizeof(T);
        return true;
    }
};

using CRoseWriter = CRosePolicy<WritePolicy>;
using CRoseSizer = CRosePolicy<SizePolicy>;

class CRoseReader {
    public:
        CRoseReader(const uint8_t *buffer, uint16_t size) : m_current(buffer), m_buffer(buffer), m_size(size) {}

        bool get_uint8_t(uint8_t& data) { return read(data); }
        bool get_int8_t(int8_t& data) { return read(data); }
        bool get_uint16_t(uint16_t& data) { return read(data); }
        bool get_int16_t(int16_t& data) { return read(data); }
        bool get_uint32_t(uint32_t& data) { return read(data); }
        bool get_int32_t(int32_t& data) { return read(data); }
        bool get_uint64_t(uint64_t& data) { return read(data); }
        bool get_int64_t(int64_t& data) { return read(data); }
        bool get_string(std::string& data) {
            char c = '\0';
            if (!read(c)) return false;
            while (c != '\0') {
                data.push_back(c);
                if (!read(c)) return false;
            }
            return true;
        }
        bool get_string(std::string& data, size_t size) {
            for (size_t i = 0; i < size; ++i) {
                char c;
                if (!read(c)) return false;
                data.push_back(c);
            }
            return true;
        }
        bool get_float(float& data) { return read(data); }
        bool get_double(double& data) { return read(data); }
        bool get_char(char& data) { return read(data); }
        bool get_iserialize(ISerialize& data) { return data.read(*this); }

        template <typename T> static T read_at(const uint8_t *const buffer) { return *reinterpret_cast<const T* const>(buffer); }

    private:
        const uint8_t *m_current, *m_buffer;
        uint16_t m_size;

        template <typename T>
        bool read(T& data) {
            static_assert(std::is_copy_assignable_v<T>, "CRoseReader doesn't know how to copy assign this type!");
            if (!m_current || m_current + sizeof(T) > m_buffer + m_size) return false;
            data = *reinterpret_cast<const T* const>(m_current);
            m_current += sizeof(T);
            return true;
        }
};

}
