#pragma once

struct CharacterGraphics {
    CharacterGraphics() : face_(0), hair_(0), race_(0) {}
    CharacterGraphics(uint8_t face, uint8_t hair, uint8_t race) : face_(face),
        hair_(hair), race_(race) {}
    template <typename T>
    CharacterGraphics(const T& row) : CharacterGraphics() {
        loadFromRow(row);
    }

    template <typename T>
    void loadFromRow(const T& row) {
        face_ = row.face;
        hair_ = row.hair;
        race_ = row.race;
    }

    uint8_t face_;
    uint8_t hair_;
    uint8_t race_;
};
