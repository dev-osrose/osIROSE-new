#include "srv_player_char.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;


void SrvPlayerChar::Header::set_type(const unsigned int type) {
    this->data.type = type;
}

unsigned int SrvPlayerChar::Header::get_type() const {
    return data.type;
}

void SrvPlayerChar::Header::set_id(const unsigned int id) {
    this->data.id = id;
}

unsigned int SrvPlayerChar::Header::get_id() const {
    return data.id;
}

void SrvPlayerChar::Header::set_isCreated(const unsigned int isCreated) {
    this->data.isCreated = isCreated;
}

unsigned int SrvPlayerChar::Header::get_isCreated() const {
    return data.isCreated;
}

void SrvPlayerChar::Header::set_header(const uint16_t header) {
    this->data.header = header;
}

uint16_t SrvPlayerChar::Header::get_header() const {
    return data.header;
}

bool SrvPlayerChar::Header::write(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(data.header)) {
        return false;
    }
    return true;
}

bool SrvPlayerChar::Header::read(CRoseReader& reader) {
    if (!reader.get_uint16_t(data.header)) {
        return false;
    }
    return true;
}

constexpr size_t SrvPlayerChar::Header::size() {
    return sizeof(data);
}

void SrvPlayerChar::Data::set_gem_opt(const unsigned int gem_opt) {
    this->data.gem_opt = gem_opt;
}

unsigned int SrvPlayerChar::Data::get_gem_opt() const {
    return data.gem_opt;
}

void SrvPlayerChar::Data::set_durability(const unsigned int durability) {
    this->data.durability = durability;
}

unsigned int SrvPlayerChar::Data::get_durability() const {
    return data.durability;
}

void SrvPlayerChar::Data::set_life(const unsigned int life) {
    this->data.life = life;
}

unsigned int SrvPlayerChar::Data::get_life() const {
    return data.life;
}

void SrvPlayerChar::Data::set_hasSocket(const unsigned int hasSocket) {
    this->data.hasSocket = hasSocket;
}

unsigned int SrvPlayerChar::Data::get_hasSocket() const {
    return data.hasSocket;
}

void SrvPlayerChar::Data::set_isAppraised(const unsigned int isAppraised) {
    this->data.isAppraised = isAppraised;
}

unsigned int SrvPlayerChar::Data::get_isAppraised() const {
    return data.isAppraised;
}

void SrvPlayerChar::Data::set_refine(const unsigned int refine) {
    this->data.refine = refine;
}

unsigned int SrvPlayerChar::Data::get_refine() const {
    return data.refine;
}

void SrvPlayerChar::Data::set_count(const uint32_t count) {
    this->data.count = count;
}

uint32_t SrvPlayerChar::Data::get_count() const {
    return data.count;
}

bool SrvPlayerChar::Data::write(CRoseBasePolicy& writer) const {
    if (!writer.set_uint32_t(data.count)) {
        return false;
    }
    return true;
}

bool SrvPlayerChar::Data::read(CRoseReader& reader) {
    if (!reader.get_uint32_t(data.count)) {
        return false;
    }
    return true;
}

constexpr size_t SrvPlayerChar::Data::size() {
    return sizeof(data);
}

void SrvPlayerChar::EquippedItem::set_id(const unsigned int id) {
    this->data.id = id;
}

unsigned int SrvPlayerChar::EquippedItem::get_id() const {
    return data.id;
}

void SrvPlayerChar::EquippedItem::set_gem_opt(const unsigned int gem_opt) {
    this->data.gem_opt = gem_opt;
}

unsigned int SrvPlayerChar::EquippedItem::get_gem_opt() const {
    return data.gem_opt;
}

void SrvPlayerChar::EquippedItem::set_socket(const unsigned int socket) {
    this->data.socket = socket;
}

unsigned int SrvPlayerChar::EquippedItem::get_socket() const {
    return data.socket;
}

void SrvPlayerChar::EquippedItem::set_grade(const unsigned int grade) {
    this->data.grade = grade;
}

unsigned int SrvPlayerChar::EquippedItem::get_grade() const {
    return data.grade;
}

void SrvPlayerChar::EquippedItem::set_data(const uint32_t data) {
    this->data.data = data;
}

uint32_t SrvPlayerChar::EquippedItem::get_data() const {
    return data.data;
}

bool SrvPlayerChar::EquippedItem::write(CRoseBasePolicy& writer) const {
    if (!writer.set_uint32_t(data.data)) {
        return false;
    }
    return true;
}

bool SrvPlayerChar::EquippedItem::read(CRoseReader& reader) {
    if (!reader.get_uint32_t(data.data)) {
        return false;
    }
    return true;
}

constexpr size_t SrvPlayerChar::EquippedItem::size() {
    return sizeof(data);
}

void SrvPlayerChar::Item::set_header(const SrvPlayerChar::Header header) {
    this->header = header;
}

SrvPlayerChar::Header SrvPlayerChar::Item::get_header() const {
    return header;
}

void SrvPlayerChar::Item::set_data(const SrvPlayerChar::Data data) {
    this->data = data;
}

SrvPlayerChar::Data SrvPlayerChar::Item::get_data() const {
    return data;
}

bool SrvPlayerChar::Item::write(CRoseBasePolicy& writer) const {
    if (!writer.set_iserialize(header)) {
        return false;
    }
    if (!writer.set_iserialize(data)) {
        return false;
    }
    return true;
}

bool SrvPlayerChar::Item::read(CRoseReader& reader) {
    if (!reader.get_iserialize(header)) {
        return false;
    }
    if (!reader.get_iserialize(data)) {
        return false;
    }
    return true;
}

constexpr size_t SrvPlayerChar::Item::size() {
    size_t size = 0;
    size += Header::size();
    size += Data::size();
    return size;
}

void SrvPlayerChar::BulletItem::set_type(const unsigned int type) {
    this->data.type = type;
}

unsigned int SrvPlayerChar::BulletItem::get_type() const {
    return data.type;
}

void SrvPlayerChar::BulletItem::set_id(const unsigned int id) {
    this->data.id = id;
}

unsigned int SrvPlayerChar::BulletItem::get_id() const {
    return data.id;
}

void SrvPlayerChar::BulletItem::set_data(const uint16_t data) {
    this->data.data = data;
}

uint16_t SrvPlayerChar::BulletItem::get_data() const {
    return data.data;
}

bool SrvPlayerChar::BulletItem::write(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(data.data)) {
        return false;
    }
    return true;
}

bool SrvPlayerChar::BulletItem::read(CRoseReader& reader) {
    if (!reader.get_uint16_t(data.data)) {
        return false;
    }
    return true;
}

constexpr size_t SrvPlayerChar::BulletItem::size() {
    return sizeof(data);
}


SrvPlayerChar::SrvPlayerChar() : CRosePacket(SrvPlayerChar::PACKET_ID) {}

SrvPlayerChar::SrvPlayerChar(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(id)) {
        return;
    }
    if (!reader.get_float(x)) {
        return;
    }
    if (!reader.get_float(y)) {
        return;
    }
    if (!reader.get_float(destX)) {
        return;
    }
    if (!reader.get_float(destY)) {
        return;
    }
    if (!reader.get_uint16_t(command)) {
        return;
    }
    if (!reader.get_uint16_t(targetId)) {
        return;
    }
    if (!reader.get_uint8_t(moveMode)) {
        return;
    }
    if (!reader.get_int32_t(hp)) {
        return;
    }
    if (!reader.get_int32_t(teamId)) {
        return;
    }
    if (!reader.get_uint32_t(statusFlag)) {
        return;
    }
    if (!reader.get_uint8_t(race)) {
        return;
    }
    if (!reader.get_uint16_t(runSpeed)) {
        return;
    }
    if (!reader.get_uint16_t(atkSpeed)) {
        return;
    }
    if (!reader.get_uint8_t(weightRate)) {
        return;
    }
    if (!reader.get_uint32_t(face)) {
        return;
    }
    if (!reader.get_uint32_t(hair)) {
        return;
    }
    for (size_t index = 0; index < MAX_VISIBLE_ITEMS; ++index) {
        if (!reader.get_iserialize(inventory[index])) {
            return;
        }
    }
    for (size_t index = 0; index < BulletType::MAX_BULLET_TYPES; ++index) {
        if (!reader.get_iserialize(bullets[index])) {
            return;
        }
    }
    if (!reader.get_uint16_t(job)) {
        return;
    }
    if (!reader.get_uint8_t(level)) {
        return;
    }
    for (size_t index = 0; index < RidingItem::MAX_RIDING_ITEMS; ++index) {
        if (!reader.get_iserialize(ridingItems[index])) {
            return;
        }
    }
    if (!reader.get_uint16_t(z)) {
        return;
    }
    if (!reader.get_uint32_t(subFlag)) {
        return;
    }
    if (!reader.get_string(name)) {
        return;
    }
    if (!reader.get_string(fakeName)) {
        return;
    }
}

void SrvPlayerChar::set_id(const uint16_t id) {
    this->id = id;
}

uint16_t SrvPlayerChar::get_id() const {
    return id;
}

void SrvPlayerChar::set_x(const float x) {
    this->x = x;
}

float SrvPlayerChar::get_x() const {
    return x;
}

void SrvPlayerChar::set_y(const float y) {
    this->y = y;
}

float SrvPlayerChar::get_y() const {
    return y;
}

void SrvPlayerChar::set_destX(const float destX) {
    this->destX = destX;
}

float SrvPlayerChar::get_destX() const {
    return destX;
}

void SrvPlayerChar::set_destY(const float destY) {
    this->destY = destY;
}

float SrvPlayerChar::get_destY() const {
    return destY;
}

void SrvPlayerChar::set_command(const uint16_t command) {
    this->command = command;
}

uint16_t SrvPlayerChar::get_command() const {
    return command;
}

void SrvPlayerChar::set_targetId(const uint16_t targetId) {
    this->targetId = targetId;
}

uint16_t SrvPlayerChar::get_targetId() const {
    return targetId;
}

void SrvPlayerChar::set_moveMode(const uint8_t moveMode) {
    this->moveMode = moveMode;
}

uint8_t SrvPlayerChar::get_moveMode() const {
    return moveMode;
}

void SrvPlayerChar::set_hp(const int32_t hp) {
    this->hp = hp;
}

int32_t SrvPlayerChar::get_hp() const {
    return hp;
}

void SrvPlayerChar::set_teamId(const int32_t teamId) {
    this->teamId = teamId;
}

int32_t SrvPlayerChar::get_teamId() const {
    return teamId;
}

void SrvPlayerChar::set_statusFlag(const uint32_t statusFlag) {
    this->statusFlag = statusFlag;
}

uint32_t SrvPlayerChar::get_statusFlag() const {
    return statusFlag;
}

void SrvPlayerChar::set_race(const uint8_t race) {
    this->race = race;
}

uint8_t SrvPlayerChar::get_race() const {
    return race;
}

void SrvPlayerChar::set_runSpeed(const uint16_t runSpeed) {
    this->runSpeed = runSpeed;
}

uint16_t SrvPlayerChar::get_runSpeed() const {
    return runSpeed;
}

void SrvPlayerChar::set_atkSpeed(const uint16_t atkSpeed) {
    this->atkSpeed = atkSpeed;
}

uint16_t SrvPlayerChar::get_atkSpeed() const {
    return atkSpeed;
}

void SrvPlayerChar::set_weightRate(const uint8_t weightRate) {
    this->weightRate = weightRate;
}

uint8_t SrvPlayerChar::get_weightRate() const {
    return weightRate;
}

void SrvPlayerChar::set_face(const uint32_t face) {
    this->face = face;
}

uint32_t SrvPlayerChar::get_face() const {
    return face;
}

void SrvPlayerChar::set_hair(const uint32_t hair) {
    this->hair = hair;
}

uint32_t SrvPlayerChar::get_hair() const {
    return hair;
}

void SrvPlayerChar::set_inventory(const std::array<SrvPlayerChar::Item, MAX_VISIBLE_ITEMS>& inventory) {
    this->inventory = inventory;
}

void SrvPlayerChar::set_inventory(const Item& inventory, size_t index) {
    this->inventory[index] = inventory;
}

const std::array<SrvPlayerChar::Item, MAX_VISIBLE_ITEMS>& SrvPlayerChar::get_inventory() const {
    return inventory;
}

const SrvPlayerChar::Item& SrvPlayerChar::get_inventory(size_t index) const {
    return inventory[index];
}

void SrvPlayerChar::set_bullets(const std::array<SrvPlayerChar::BulletItem, BulletType::MAX_BULLET_TYPES>& bullets) {
    this->bullets = bullets;
}

void SrvPlayerChar::set_bullets(const BulletItem& bullets, size_t index) {
    this->bullets[index] = bullets;
}

const std::array<SrvPlayerChar::BulletItem, BulletType::MAX_BULLET_TYPES>& SrvPlayerChar::get_bullets() const {
    return bullets;
}

const SrvPlayerChar::BulletItem& SrvPlayerChar::get_bullets(size_t index) const {
    return bullets[index];
}

void SrvPlayerChar::set_job(const uint16_t job) {
    this->job = job;
}

uint16_t SrvPlayerChar::get_job() const {
    return job;
}

void SrvPlayerChar::set_level(const uint8_t level) {
    this->level = level;
}

uint8_t SrvPlayerChar::get_level() const {
    return level;
}

void SrvPlayerChar::set_ridingItems(const std::array<SrvPlayerChar::EquippedItem, RidingItem::MAX_RIDING_ITEMS>& ridingItems) {
    this->ridingItems = ridingItems;
}

void SrvPlayerChar::set_ridingItems(const EquippedItem& ridingItems, size_t index) {
    this->ridingItems[index] = ridingItems;
}

const std::array<SrvPlayerChar::EquippedItem, RidingItem::MAX_RIDING_ITEMS>& SrvPlayerChar::get_ridingItems() const {
    return ridingItems;
}

const SrvPlayerChar::EquippedItem& SrvPlayerChar::get_ridingItems(size_t index) const {
    return ridingItems[index];
}

void SrvPlayerChar::set_z(const uint16_t z) {
    this->z = z;
}

uint16_t SrvPlayerChar::get_z() const {
    return z;
}

void SrvPlayerChar::set_subFlag(const uint32_t subFlag) {
    this->subFlag = subFlag;
}

uint32_t SrvPlayerChar::get_subFlag() const {
    return subFlag;
}

void SrvPlayerChar::set_name(const std::string& name) {
    this->name = name;
}

const std::string& SrvPlayerChar::get_name() const {
    return name;
}

void SrvPlayerChar::set_fakeName(const std::string& fakeName) {
    this->fakeName = fakeName;
}

const std::string& SrvPlayerChar::get_fakeName() const {
    return fakeName;
}

SrvPlayerChar SrvPlayerChar::create(const uint16_t& id) {
    SrvPlayerChar packet;
    packet.set_id(id);
    return packet;
}

SrvPlayerChar SrvPlayerChar::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvPlayerChar(reader);
}

std::unique_ptr<SrvPlayerChar> SrvPlayerChar::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvPlayerChar>(reader);
}

void SrvPlayerChar::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(id)) {
        return;
    }
    if (!writer.set_float(x)) {
        return;
    }
    if (!writer.set_float(y)) {
        return;
    }
    if (!writer.set_float(destX)) {
        return;
    }
    if (!writer.set_float(destY)) {
        return;
    }
    if (!writer.set_uint16_t(command)) {
        return;
    }
    if (!writer.set_uint16_t(targetId)) {
        return;
    }
    if (!writer.set_uint8_t(moveMode)) {
        return;
    }
    if (!writer.set_int32_t(hp)) {
        return;
    }
    if (!writer.set_int32_t(teamId)) {
        return;
    }
    if (!writer.set_uint32_t(statusFlag)) {
        return;
    }
    if (!writer.set_uint8_t(race)) {
        return;
    }
    if (!writer.set_uint16_t(runSpeed)) {
        return;
    }
    if (!writer.set_uint16_t(atkSpeed)) {
        return;
    }
    if (!writer.set_uint8_t(weightRate)) {
        return;
    }
    if (!writer.set_uint32_t(face)) {
        return;
    }
    if (!writer.set_uint32_t(hair)) {
        return;
    }
    for (const auto& elem : inventory) {
        if (!writer.set_iserialize(elem)) {
            return;
        }
    }
    for (const auto& elem : bullets) {
        if (!writer.set_iserialize(elem)) {
            return;
        }
    }
    if (!writer.set_uint16_t(job)) {
        return;
    }
    if (!writer.set_uint8_t(level)) {
        return;
    }
    for (const auto& elem : ridingItems) {
        if (!writer.set_iserialize(elem)) {
            return;
        }
    }
    if (!writer.set_uint16_t(z)) {
        return;
    }
    if (!writer.set_uint32_t(subFlag)) {
        return;
    }
    if (!writer.set_string(name)) {
        return;
    }
    if (!writer.set_string(fakeName)) {
        return;
    }
}

constexpr size_t SrvPlayerChar::size() {
    size_t size = 0;
    size += sizeof(uint16_t);
    size += sizeof(float);
    size += sizeof(float);
    size += sizeof(float);
    size += sizeof(float);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += sizeof(uint8_t);
    size += sizeof(int32_t);
    size += sizeof(int32_t);
    size += sizeof(uint32_t);
    size += sizeof(uint8_t);
    size += sizeof(uint16_t);
    size += sizeof(uint16_t);
    size += sizeof(uint8_t);
    size += sizeof(uint32_t);
    size += sizeof(uint32_t);
    size += Item::size() * MAX_VISIBLE_ITEMS;
    size += BulletItem::size() * BulletType::MAX_BULLET_TYPES;
    size += sizeof(uint16_t);
    size += sizeof(uint8_t);
    size += EquippedItem::size() * RidingItem::MAX_RIDING_ITEMS;
    size += sizeof(uint16_t);
    size += sizeof(uint32_t);
    return size;
}

