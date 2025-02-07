#pragma once

#include <cstdint>
#include <optional>
#include <ostream>
#include <vector>

enum Piece : int8_t {
    None = 0,
    Rook = 1,
    Knigt = 2,
    Bishop = 3,
    Queen = 4,
    King = 5,
    Pawn = 6,
};

enum Colour : int8_t {
    White = 8,
    Black = 16,
};

inline const char* to_string(Colour c) {
    switch (c) {
    case White:
        return "White";
    case Black:
        return "Black";
    default:
        return "[Unknown Colour]";
    }
}

inline const char* to_string(Piece c) {
    switch (c) {
    case Pawn:
        return "Pawn";
    case Rook:
        return "Rook";
    case Bishop:
        return "Bishop";
    case Knigt:
        return "Knight";
    case Queen:
        return "Queen";
    case King:
        return "King";
    default:
        return "[Unknown Colour]";
    }
}

inline std::ostream& operator<<(std::ostream& os, const Colour& i) {
    os << std::to_string(i);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Piece& i) {
    os << std::to_string(i);
    return os;
}

class Board;
struct Coords;
struct Move;

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
