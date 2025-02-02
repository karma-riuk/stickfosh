#pragma once

#include "pieces/piece.hpp"

#include <string>

struct Coords {
    int x, y;

    int8_t to_index() const {
        return this->y * 8 + this->x;
    }

    static Coords from_index(int idx) {
        return {idx % 8, idx / 8};
    }
};

struct Board {
    int8_t squares[64] = {Piece::None};
    bool white_to_play;
    int8_t w_castle_rights;
    int8_t b_castle_rights;
    static Board setup_fen_position(std::string fen);

    Board make_move(Move);
    std::string to_fen();
};
