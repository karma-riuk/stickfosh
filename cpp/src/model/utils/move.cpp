#include "move.hpp"

#include "../board/board.hpp"
#include "utils.hpp"

#include <algorithm>

int Move::score_guess(const Board& b) const {
    int ret = 0;

    Piece me_piece = b.piece_at(source_square);
    Piece captured_piece = b.piece_at(target_square);

    if (captured_piece != Piece::None)
        ret += 10 * piece_value(captured_piece) - piece_value(me_piece);

    if (promoting_to != Piece::None)
        ret += piece_value(promoting_to);

    std::vector<int8_t> pawn_attack_map = b.opponent_pawn_attack_map();
    if (std::find(pawn_attack_map.begin(), pawn_attack_map.end(), target_square)
        != pawn_attack_map.end())
        ret -= me_piece;

    return ret;
}
