#pragma once

#include "../move.hpp"

#include <cstdint>
#include <vector>

enum Piece : int8_t {
    None = 0,
    King = 1,
    Pawn = 2,
    Knigt = 3,
    Bishop = 4,
    Rook = 5,
    Queen = 6,
};

enum Colour : int8_t {
    White = 8,
    Black = 16,
};

class Board;
std::vector<Move> legal_moves(Piece p, Board b);
