#pragma once

#include "../move.hpp"

#include <cstdint>
#include <optional>
#include <ostream>
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

inline std::ostream& operator<<(std::ostream& os, const int8_t& i) {
    os << std::to_string(i);
    return os;
}

class Board;
struct Coords;

std::vector<Move> legal_moves(int8_t, const Board&, const Coords, bool = false);
std::vector<Move> keep_only_blocking(const std::vector<Move>, const Board&);
std::optional<Move> move_for_position(const Board&, const Coords, const Coords);
std::vector<Move> look_direction(const Board&, const Coords, int, int);

std::vector<Move> pawn_moves(const Board&, const Coords);
std::vector<Move> rook_moves(const Board&, const Coords);
std::vector<Move> knight_moves(const Board&, const Coords);
std::vector<Move> bishop_moves(const Board&, const Coords);
std::vector<Move> queen_moves(const Board&, const Coords);
std::vector<Move> king_moves(const Board&, const Coords);
