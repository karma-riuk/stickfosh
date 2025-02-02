#include "castle_side.hpp"

#include <cstdint>

struct Move {
    int8_t piece;
    int8_t target_square;

    bool is_capturing = false;
    CastleSide castle_side = CastleSide::Neither;
};
