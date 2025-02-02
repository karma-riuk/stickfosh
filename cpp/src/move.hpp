#pragma once

#include "castle_side.hpp"

#include <cstdint>

struct Move {
    int8_t source_square;
    int8_t target_square;

    bool is_capturing = false;
    CastleSide castle_side = CastleSide::NeitherSide;
    bool en_passant = false;
    int8_t promoting_to = 0;
};
