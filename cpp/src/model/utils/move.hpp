#pragma once

#include "../board/castle_side.hpp"
#include "../pieces/piece.hpp"
#include "coords.hpp"

#include <cstdint>
#include <sstream>

struct Move {
    int8_t source_square;
    int8_t target_square;

    int8_t promoting_to = Piece::None;

    std::string to_string() const {
        std::stringstream ss;
        ss << Coords::from_index(source_square)
           << Coords::from_index(target_square);
        if (promoting_to != Piece::None) {
            switch (promoting_to & 0b00111) {
            case Queen:
                ss << 'q';
                break;
            case Bishop:
                ss << 'b';
                break;
            case Knigt:
                ss << 'n';
                break;
            case Rook:
                ss << 'r';
                break;
            default:
                break;
            }
        }
        return ss.str();
    }
};

inline bool operator<(const Move& m1, const Move& m2) {
    return m1.to_string() < m2.to_string();
}

inline std::ostream& operator<<(std::ostream& os, const Move& m) {
    os << m.to_string();
    return os;
}
