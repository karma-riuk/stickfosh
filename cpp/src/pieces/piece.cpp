#include "piece.hpp"

#include "../board.hpp"

std::vector<Move> legal_moves(
    const Piece p,
    const Board& b,
    const Coords xy,
    bool looking_for_check = false
) {
    std::vector<Move> ret;
    switch (p) {
    case Piece::Pawn:
        ret = pawn_moves(b, xy);
    case Piece::Bishop:
        break;
    default:
        break;
    }

    if (!looking_for_check)
        return keep_only_blocking(ret, b);

    return {};
}
