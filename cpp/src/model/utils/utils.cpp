#include "utils.hpp"

#include "../board/board.hpp"

std::vector<int8_t> to_target_square(std::vector<Move> moves) {
    std::vector<int8_t> ret;
    for (Move move : moves)
        ret.push_back(move.target_square);
    return ret;
}

int count_material(const Board& b, int8_t colour) {
    int ret = 0;
    for (int i = 0; i < 64; i++) {
        if (b.colour_at(i) == colour)
            switch (b.piece_at(i)) {
            case Piece::Pawn:
                ret += PawnValue;
                break;
            case Piece::Knigt:
                ret += KnightValue;
                break;
            case Piece::Bishop:
                ret += BishopValue;
                break;
            case Piece::Rook:
                ret += RookValue;
                break;
            case Piece::Queen:
                ret += QueenValue;
                break;
            case Piece::King:
            case Piece::None:
                break;
            }
    }
    return ret;
}
