#include "srv_quest_data.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;


void SrvQuestData::Header::set_isCreated(const unsigned int isCreated) {
    this->data.isCreated = isCreated;
}

unsigned int SrvQuestData::Header::get_isCreated() const {
    return data.isCreated;
}

void SrvQuestData::Header::set_id(const unsigned int id) {
    this->data.id = id;
}

unsigned int SrvQuestData::Header::get_id() const {
    return data.id;
}

void SrvQuestData::Header::set_type(const unsigned int type) {
    this->data.type = type;
}

unsigned int SrvQuestData::Header::get_type() const {
    return data.type;
}

void SrvQuestData::Header::set_header(const uint16_t header) {
    this->data.header = header;
}

uint16_t SrvQuestData::Header::get_header() const {
    return data.header;
}

bool SrvQuestData::Header::write(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(data.header)) {
        return false;
    }
    return true;
}

bool SrvQuestData::Header::read(CRoseReader& reader) {
    if (!reader.get_uint16_t(data.header)) {
        return false;
    }
    return true;
}

constexpr size_t SrvQuestData::Header::size() {
    return sizeof(data);
}

void SrvQuestData::Quest::set_id(const uint16_t id) {
    this->id = id;
}

uint16_t SrvQuestData::Quest::get_id() const {
    return id;
}

void SrvQuestData::Quest::set_timer(const uint32_t timer) {
    this->timer = timer;
}

uint32_t SrvQuestData::Quest::get_timer() const {
    return timer;
}

void SrvQuestData::Quest::set_vars(const uint32_t* vars) {
    for (size_t index = 0; index < MAX_QUEST_VARS; ++index) {
        this->vars[index] = vars[index];
    }
}

void SrvQuestData::Quest::set_vars(const uint32_t vars, size_t index) {
    this->vars[index] = vars;
}

const uint32_t* SrvQuestData::Quest::get_vars() const {
    return vars;
}

uint32_t SrvQuestData::Quest::get_vars(size_t index) const {
    return vars[index];
}

void SrvQuestData::Quest::set_switches(const uint32_t switches) {
    this->switches = switches;
}

uint32_t SrvQuestData::Quest::get_switches() const {
    return switches;
}

void SrvQuestData::Quest::set_items(const Item* items) {
    for (size_t index = 0; index < MAX_QUEST_ITEMS; ++index) {
        this->items[index] = items[index];
    }
}

void SrvQuestData::Quest::set_items(const Item items, size_t index) {
    this->items[index] = items;
}

const Item* SrvQuestData::Quest::get_items() const {
    return items;
}

Item SrvQuestData::Quest::get_items(size_t index) const {
    return items[index];
}

bool SrvQuestData::Quest::write(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(id)) {
        return false;
    }
    if (!writer.set_uint32_t(timer)) {
        return false;
    }
    for (size_t index = 0; index < MAX_QUEST_VARS; ++index) {
        if (!writer.set_uint32_t(vars[index])) {
            return false;
        }
    }
    if (!writer.set_uint32_t(switches)) {
        return false;
    }
    for (size_t index = 0; index < MAX_QUEST_ITEMS; ++index) {
        if (!writer.set_Item(items[index])) {
            return false;
        }
    }
    return true;
}

bool SrvQuestData::Quest::read(CRoseReader& reader) {
    if (!reader.get_uint16_t(id)) {
        return false;
    }
    if (!reader.get_uint32_t(timer)) {
        return false;
    }
    for (size_t index = 0; index < MAX_QUEST_VARS; ++index) {
        if (!reader.get_uint32_t(vars[index])) {
            return false;
        }
    }
    if (!reader.get_uint32_t(switches)) {
        return false;
    }
    for (size_t index = 0; index < MAX_QUEST_ITEMS; ++index) {
        if (!reader.get_Item(items[index])) {
            return false;
        }
    }
    return true;
}

constexpr size_t SrvQuestData::Quest::size() {
    size_t size = 0;
    size += sizeof(uint16_t);
    size += sizeof(uint32_t);
    size += sizeof(uint32_t) * MAX_QUEST_VARS;
    size += sizeof(uint32_t);
    size += sizeof(Item) * MAX_QUEST_ITEMS;
    return size;
}


SrvQuestData::SrvQuestData() : CRosePacket(ePacketType::PAKWC_QUEST_DATA) {}

SrvQuestData::SrvQuestData(CRoseReader reader) : CRosePacket(reader) {
    for (size_t index = 0; index < MAX_CONDITIONS_EPISODE; ++index) {
        if (!reader.get_uint16_t(episode[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_CONDTIONS_JOB; ++index) {
        if (!reader.get_uint16_t(job[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_CONDITIONS_PLANET; ++index) {
        if (!reader.get_uint16_t(planet[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_CONDITIONS_UNION; ++index) {
        if (!reader.get_uint16_t(union[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_QUESTS; ++index) {
        if (!reader.get_iserialize(quests[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_SWITCHES; ++index) {
        if (!reader.get_uint32_t(switches[index])) {
            return;
        }
    }
}

void SrvQuestData::set_episode(const uint16_t* episode) {
    for (size_t index = 0; index < MAX_CONDITIONS_EPISODE; ++index) {
        this->episode[index] = episode[index];
    }
}

void SrvQuestData::set_episode(const uint16_t episode, size_t index) {
    this->episode[index] = episode;
}

const uint16_t* SrvQuestData::get_episode() const {
    return episode;
}

uint16_t SrvQuestData::get_episode(size_t index) const {
    return episode[index];
}

void SrvQuestData::set_job(const uint16_t* job) {
    for (size_t index = 0; index < MAX_CONDTIONS_JOB; ++index) {
        this->job[index] = job[index];
    }
}

void SrvQuestData::set_job(const uint16_t job, size_t index) {
    this->job[index] = job;
}

const uint16_t* SrvQuestData::get_job() const {
    return job;
}

uint16_t SrvQuestData::get_job(size_t index) const {
    return job[index];
}

void SrvQuestData::set_planet(const uint16_t* planet) {
    for (size_t index = 0; index < MAX_CONDITIONS_PLANET; ++index) {
        this->planet[index] = planet[index];
    }
}

void SrvQuestData::set_planet(const uint16_t planet, size_t index) {
    this->planet[index] = planet;
}

const uint16_t* SrvQuestData::get_planet() const {
    return planet;
}

uint16_t SrvQuestData::get_planet(size_t index) const {
    return planet[index];
}

void SrvQuestData::set_union(const uint16_t* union) {
    for (size_t index = 0; index < MAX_CONDITIONS_UNION; ++index) {
        this->union[index] = union[index];
    }
}

void SrvQuestData::set_union(const uint16_t union, size_t index) {
    this->union[index] = union;
}

const uint16_t* SrvQuestData::get_union() const {
    return union;
}

uint16_t SrvQuestData::get_union(size_t index) const {
    return union[index];
}

void SrvQuestData::set_quests(const SrvQuestData::Quest* quests) {
    for (size_t index = 0; index < MAX_QUESTS; ++index) {
        this->quests[index] = quests[index];
    }
}

void SrvQuestData::set_quests(const Quest quests, size_t index) {
    this->quests[index] = quests;
}

const SrvQuestData::Quest* SrvQuestData::get_quests() const {
    return quests;
}

SrvQuestData::Quest SrvQuestData::get_quests(size_t index) const {
    return quests[index];
}

void SrvQuestData::set_switches(const uint32_t* switches) {
    for (size_t index = 0; index < MAX_SWITCHES; ++index) {
        this->switches[index] = switches[index];
    }
}

void SrvQuestData::set_switches(const uint32_t switches, size_t index) {
    this->switches[index] = switches;
}

const uint32_t* SrvQuestData::get_switches() const {
    return switches;
}

uint32_t SrvQuestData::get_switches(size_t index) const {
    return switches[index];
}

SrvQuestData SrvQuestData::create() {
    SrvQuestData packet;
    return packet;
}

SrvQuestData SrvQuestData::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvQuestData(reader);
}

std::unique_ptr<SrvQuestData> SrvQuestData::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvQuestData>(reader);
}

void SrvQuestData::pack(CRoseBasePolicy& writer) const {
    for (size_t index = 0; index < MAX_CONDITIONS_EPISODE; ++index) {
        if (!writer.set_uint16_t(episode[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_CONDTIONS_JOB; ++index) {
        if (!writer.set_uint16_t(job[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_CONDITIONS_PLANET; ++index) {
        if (!writer.set_uint16_t(planet[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_CONDITIONS_UNION; ++index) {
        if (!writer.set_uint16_t(union[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_QUESTS; ++index) {
        if (!writer.set_iserialize(quests[index])) {
            return;
        }
    }
    for (size_t index = 0; index < MAX_SWITCHES; ++index) {
        if (!writer.set_uint32_t(switches[index])) {
            return;
        }
    }
}

constexpr size_t SrvQuestData::size() {
    size_t size = 0;
    size += sizeof(uint16_t) * MAX_CONDITIONS_EPISODE;
    size += sizeof(uint16_t) * MAX_CONDTIONS_JOB;
    size += sizeof(uint16_t) * MAX_CONDITIONS_PLANET;
    size += sizeof(uint16_t) * MAX_CONDITIONS_UNION;
    size += Quest::size() * MAX_QUESTS;
    size += sizeof(uint32_t) * MAX_SWITCHES;
    return size;
}

