#pragma once

#include "iserialize.h"
#include <bitset>

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

        template <size_t N>
        bool set_bitset(const std::bitset<N>& bitset) = delete;

        virtual uint16_t get_size() const = 0;
};

template <> inline
bool CRoseBasePolicy::set_bitset<8>(const std::bitset<8>& bitset) {
    return set_uint8_t(bitset.to_ulong());
}

template <> inline
bool CRoseBasePolicy::set_bitset<16>(const std::bitset<16>& bitset) {
    return set_uint16_t(bitset.to_ulong());
}

template <> inline
bool CRoseBasePolicy::set_bitset<24>(const std::bitset<24>& bitset) {
    const bool res = set_uint16_t(bitset.to_ulong());
    return res && set_uint8_t(bitset.to_ulong() >> 16);
}

template <> inline
bool CRoseBasePolicy::set_bitset<32>(const std::bitset<32>& bitset) {
    return set_uint32_t(bitset.to_ulong());
}

template <> inline
bool CRoseBasePolicy::set_bitset<40>(const std::bitset<40>& bitset) {
    const bool res = set_uint32_t(bitset.to_ullong());
    return res && set_uint8_t(bitset.to_ullong() >> 32);
}

template <> inline
bool CRoseBasePolicy::set_bitset<48>(const std::bitset<48>& bitset) {
    const bool res = set_uint32_t(bitset.to_ullong());
    return res && set_uint16_t(bitset.to_ullong() >> 32);
}

template <> inline
bool CRoseBasePolicy::set_bitset<56>(const std::bitset<56>& bitset) {
    bool res = set_uint32_t(bitset.to_ullong());
    res = res && set_uint16_t(bitset.to_ullong() >> 32);
    return res && set_uint8_t(bitset.to_ullong() >> 48);
}

template <> inline
bool CRoseBasePolicy::set_bitset<64>(const std::bitset<64>& bitset) {
    return set_uint64_t(bitset.to_ullong());
}

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

        template <size_t N>
        bool get_bitset(std::bitset<N>& bitset) = delete;

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

template <> inline
bool CRoseReader::get_bitset<8>(std::bitset<8>& bitset) {
    uint8_t tmp;
    const bool res = get_uint8_t(tmp);
    if (res) {
        bitset = std::bitset<8>(tmp);
    }
    return res;
}

template <> inline
bool CRoseReader::get_bitset<16>(std::bitset<16>& bitset) {
    uint16_t tmp;
    const bool res = get_uint16_t(tmp);
    if (res) {
        bitset = std::bitset<16>(tmp);
    }
    return res;
}

template <> inline
bool CRoseReader::get_bitset<24>(std::bitset<24>& bitset) {
    uint16_t tmp;
    if (!get_uint16_t(tmp)) {
        return false;
    }
    bitset = std::bitset<24>(tmp);
    bitset << 8;
    uint8_t tmp2;
    if (!get_uint8_t(tmp2)) {
        return false;
    }
    bitset |= std::bitset<24>(tmp2);
    return true;
}

template <> inline
bool CRoseReader::get_bitset<32>(std::bitset<32>& bitset) {
    uint32_t tmp;
    const bool res = get_uint32_t(tmp);
    if (res) {
        bitset = std::bitset<32>(tmp);
    }
    return res;
}

template <> inline
bool CRoseReader::get_bitset<40>(std::bitset<40>& bitset) {
    uint32_t tmp;
    if (!get_uint32_t(tmp)) {
        return false;
    }
    bitset = std::bitset<40>(tmp);
    bitset << 8;
    uint8_t tmp2;
    if (!get_uint8_t(tmp2)) {
        return false;
    }
    bitset |= std::bitset<40>(tmp2);
    return true;
}

template <> inline
bool CRoseReader::get_bitset<48>(std::bitset<48>& bitset) {
    uint32_t tmp;
    if (!get_uint32_t(tmp)) {
        return false;
    }
    bitset = std::bitset<48>(tmp);
    bitset << 16;
    uint16_t tmp2;
    if (!get_uint16_t(tmp2)) {
        return false;
    }
    bitset |= std::bitset<48>(tmp2);
    return true;
}

template <> inline
bool CRoseReader::get_bitset<56>(std::bitset<56>& bitset) {
    uint32_t tmp;
    if (!get_uint32_t(tmp)) {
        return false;
    }
    bitset = std::bitset<56>(tmp);
    bitset << (16 + 8);
    uint16_t tmp2;
    if (!get_uint16_t(tmp2)) {
        return false;
    }
    bitset |= std::bitset<56>(tmp2) << 8;
    uint8_t tmp3;
    if (!get_uint8_t(tmp3)) {
        return false;
    }
    bitset |= std::bitset<56>(tmp3);
    return true;
}

template <> inline
bool CRoseReader::get_bitset<64>(std::bitset<64>& bitset) {
    uint64_t tmp;
    if (!get_uint64_t(tmp)) {
        return false;
    }
    bitset = std::bitset<64>(tmp);
    return true;
}

}
