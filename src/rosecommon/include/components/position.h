#pragma once

namespace Component {
struct Position {
    float x;
    float y;
    uint16_t z;
    uint16_t spawn;
    float npcAngle = 0; // angle for npcs
};
}
