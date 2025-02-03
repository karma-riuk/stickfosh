#pragma once

#include "castle_side.hpp"
#include "coords.hpp"
#include "pieces/piece.hpp"

#include <cstdint>

struct Move {
    int8_t source_square;
    int8_t target_square;

    bool is_capturing = false;
    CastleSide castle_side = CastleSide::NeitherSide;
    bool en_passant = false;
    int8_t promoting_to = Piece::None;
};

inline std::ostream& operator<<(std::ostream& os, const Move& m) {
    os << Coords::from_index(m.source_square)
       << Coords::from_index(m.target_square);
    return os;
}
