#pragma once

#include "epackettype.h"
#include "iserialize.h"
#include "crosewriter.h"
#include <chrono>

namespace RoseCommon {
namespace {
using namespace std::chrono_literals;
}

struct HotbarItem : public ISerialize {
    virtual bool read(CRoseReader& reader) override {
        if (!reader.get_uint16_t(data.item)) {
            return false;
        }
        return true;
    }

    virtual bool write(CRoseBasePolicy& writer) const override {
        if (!writer.set_uint16_t(data.item)) {
            return false;
        }
        return true;
    }
    
    static constexpr size_t size() { return sizeof(data); }
    
    void set_type(uint8_t type) { data.type = type; }
    uint8_t get_type() const { return data.type; };
    void set_slotId(uint16_t id) { data.slotId = id; }
    uint16_t get_slotId() const { return data.slotId; }
    
    private:
        union {
            PACK(struct {
                uint8_t type : 5;
                uint16_t slotId : 11;
            });
            uint16_t item = 0;
        } data;
};

struct Skill : public ISerialize {
    virtual bool read(CRoseReader& reader) override {
        if (!reader.get_uint16_t(id)) {
            return false;
        }
        if (!reader.get_uint8_t(level)) {
            return false;
        }
        return true;
    }

    virtual bool write(CRoseBasePolicy& writer) const override {
        if (!writer.set_uint16_t(id)) {
            return false;
        }
        if (!writer.set_uint8_t(level)) {
            return false;
        }
        return true;
    }
    
    static constexpr size_t size() { return sizeof(id) + sizeof(level); }
    
    void set_id(uint16_t id) { this->id = id; }
    uint16_t get_id() const { return id; }
    void set_level(uint8_t level) { this->level = level; }
    uint8_t get_level() const { return level; }
    
    private:
        uint16_t id = 0;
        uint8_t level = 0;
};

struct StatusEffect : public ISerialize {
    virtual bool read(CRoseReader& reader) override {
        uint32_t count;
        if (!reader.get_uint32_t(count)) {
            return false;
        }
        expired = std::chrono::seconds(count);
        if (!reader.get_uint16_t(value)) {
            return false;
        }
        if (!reader.get_uint16_t(unkown)) {
            return false;
        }
        return true;
    }

    virtual bool write(CRoseBasePolicy& writer) const override {
        if (!writer.set_uint32_t(expired.count())) {
            return false;
        }
        if (!writer.set_uint16_t(value)) {
            return false;
        }
        if (!writer.set_uint16_t(unkown)) {
            return false;
        }
        return true;
    }
    
    static constexpr size_t size() { return sizeof(uint32_t) + sizeof(value) + sizeof(unkown); }

    void set_expired(std::chrono::seconds expired) { this->expired = expired; }
    std::chrono::seconds get_expired() const { return expired; }
    void set_value(uint16_t value) { this->value = value; }
    uint16_t get_value() const { return value; }
    void set_unkown(uint16_t unkown) { this->unkown = unkown; }
    uint16_t get_unkown() const { return unkown; }
    void set_dt(std::chrono::milliseconds dt) { this->dt = dt; }
    std::chrono::milliseconds get_dt() const { return dt; }
    
    private:
        std::chrono::seconds expired = 0s;
        uint16_t value = 0;
        uint16_t unkown = 0;
        std::chrono::milliseconds dt = 0ms;
};
}
