#include "piece.hpp"

#include "../board.hpp"

std::vector<Move> legal_moves(Piece p, Board b) {
    switch (p) {
    case Piece::Pawn:
        return pawn_moves(b);
    case Piece::Bishop:
        break;
    default:
        break;
    }
    return {};
}
