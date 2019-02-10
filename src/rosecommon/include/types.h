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

struct PartyData : public ISerialize {
    virtual bool read(CRoseReader& reader) override {
        int8_t size = 0;
        if (!reader.get_int8_t(size)) return false;
        if (size == -1) { // it's a leaver
            if (!reader.get_uint32_t(tag_leaver)) return false;
            if (!reader.get_uint32_t(tag_leader)) return false;
        } else { // it's an add
            for (int8_t i = 0; i < size; ++i) {
                MemberData member;
                if (!reader.get_iserialize(member)) return false;
                members.push_back(member);
            }
        }
        return true;
    }

    virtual bool write(CRoseBasePolicy& writer) const override {
        if (is_delete) {
            if (!writer.set_int8_t(-1)) return false;
            if (!writer.set_uint32_t(tag_leaver)) return false;
            if (!writer.set_uint32_t(tag_leader)) return false;
        } else {
            if (!writer.set_int8_t(members.size())) return false;
            for (const auto& it : members) {
                if (!writer.set_iserialize(it)) return falsem
            }
        }
        return true;
    }
    
    static constexpr size_t size() { return sizeof(int8_t); }

    void set_tagLeaver(uint32_t leaver) {
        is_delete = true;
        tag_leaver = leaver;
    }

    void set_tagLeader(uint32_t leader) {
        is_delete = true;
        tag_leader = leader;
    }

    struct MemberData : public ISerialize {
        virtual bool read(CRoseReader& reader) override {
            if (!reader.get_uint32_t(tag)) return false;
            if (!reader.get_uint16_t(id)) return false;
            if (!reader.get_int32_t(max_hp)) return false;
            if (!reader.get_int32_t(hp)) return false;
            if (!reader.get_uint32_t(status_flag)) return false;
            if (!reader.get_uint16_t(con)) return false;
            if (!reader.get_uint16_t(recovery_hp)) return false;
            if (!reader.get_uint16_t(recovery_mp)) return false;
            if (!reader.get_uint16_t(stamina)) return false;
            if (!reader.get_string(name)) return false;
            return true;
        }

        virtual bool write(CRoseBasePolicy& writer) const override {
            if (!writer.set_uint32_t(tag)) return false;
            if (!writer.set_uint16_t(id)) return false;
            if (!writer.set_int32_t(max_hp)) return false;
            if (!writer.set_int32_t(hp)) return false;
            if (!writer.set_uint32_t(status_flag)) return false;
            if (!writer.set_uint16_t(con)) return false;
            if (!writer.set_uint16_t(recovery_hp)) return false;
            if (!writer.set_uint16_t(recovery_mp)) return false;
            if (!writer.set_uint16_t(stamina)) return false;
            if (!writer.set_string(name)) return false;
            return true;
        }

        uint32_t get_tag() const { return tag; }
        uint16_t get_id() const { return id; }
        int32_t get_maxHp() const { return max_hp; }
        int32_t get_hp() const { return hp; }
        uint32_t get_statusFlag() const { return status_flag; }
        uint16_t get_con() const { return con; }
        uint16_t get_recoveryHp() const { return recovery_hp; }
        uint16_t get_recoveryMp() const { return recovery_mp; }
        uint16_t get_stamina() const { return stamina; }
        const std::string& get_name() const { return name; }

        void set_tag(uint32_t data) { tag = data; }
        void set_id(uint16_t data) { id = data; }
        void set_maxHp(int32_t data) { max_hp = data; }
        void set_hp(int32_t data) { hp = data; }
        void set_statusFlag(uint32_t data) { status_flag = data; }
        void set_con(uint16_t data) { con = data; }
        void set_recoveryHp(uint16_t data) { recovery_hp = data; }
        void set_recoveryMp(uint16_t data) { recovery_mp = data; }
        void set_stamina(uint16_t data) { stamina = data; }
        void set_name(const std::string& data) { name = data; }

        private:
            uint32_t tag;
            uint16_t id;
            int32_t max_hp;
            int32_t hp;
            uint32_t status_flag;
            uint16_t con;
            uint16_t recovery_hp;
            uint16_t recovery_mp;
            uint16_t stamina;
            std::string name;
    };

    void add_member(MemberData member) {
        members.push_back(member);
    }

    private:
        bool is_delete = false;
        uint32_t tag_leaver;
        uint32_t tag_leader;
        std::vector<MemberData> members;
};

}
