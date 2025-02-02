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
struct Coords;

std::vector<Move> legal_moves(const Piece, const Board&, const Coords);

std::vector<Move> pawn_moves(const Board&, const Coords);
std::vector<Move> rook_moves(const Board&, const Coords);
std::vector<Move> knight_moves(const Board&, const Coords);
std::vector<Move> bishop_moves(const Board&, const Coords);
std::vector<Move> queen_moves(const Board&, const Coords);
std::vector<Move> king_moves(const Board&, const Coords);
