#include "piece.hpp"

#include "../board.hpp"

std::vector<Move> legal_moves(const Piece p, const Board& b, const Coords xy) {
    switch (p) {
    case Piece::Pawn:
        return pawn_moves(b, xy);
    case Piece::Bishop:
        break;
    default:
        break;
    }
    return {};
}
