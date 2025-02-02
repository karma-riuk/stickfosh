#include "piece.hpp"

#include "../board.hpp"

std::vector<Move>
keep_only_blocking(const std::vector<Move> candidates, const Board& board) {
    if (candidates.size() == 0)
        return {};

    int8_t my_colour = board.squares[candidates[0].source_square] & 0b11000;
    std::vector<Move> ret;
    for (Move move : candidates) {
        Board board_after_move = board.make_move(move);
        if (!board_after_move.is_check_for(my_colour))
            ret.push_back(move);
    }
    return ret;
}

std::vector<Move> legal_moves(
    int8_t p, const Board& b, const Coords xy, bool looking_for_check = false
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
