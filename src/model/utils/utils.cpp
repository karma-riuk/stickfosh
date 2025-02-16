#include "utils.hpp"

#include "../board/board.hpp"

std::vector<int8_t> to_target_square(std::vector<Move> moves) {
    std::vector<int8_t> ret;
    for (Move move : moves)
        ret.push_back(move.target_square);
    return ret;
}

int piece_value(Piece p) {
    switch (p) {
    case Piece::Pawn:
        return PawnValue;
    case Piece::Knigt:
        return KnightValue;
    case Piece::Bishop:
        return BishopValue;
    case Piece::Rook:
        return RookValue;
    case Piece::Queen:
        return QueenValue;
    default:
        return 0;
    }
}

int count_material(const Board& b, int8_t colour) {
    int ret = 0;
    for (int i = 0; i < 64; i++)
        if (b.colour_at(i) == colour)
            ret += piece_value(b.piece_at(i));
    return ret;
}
