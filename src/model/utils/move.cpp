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

Move Move::from_string(std::string move) {
    if (!(4 <= move.size() && move.size() <= 5))
        throw std::invalid_argument("Move must be 4 or 5 characters long");
    Move ret;
    ret.source_square = Coords::from_algebraic(move.substr(0, 2)).to_index();
    ret.target_square = Coords::from_algebraic(move.substr(2, 2)).to_index();
    if (move.size() == 5)
        switch (move[4]) {
        case 'n':
            ret.promoting_to = Knigt;
            break;
        case 'b':
            ret.promoting_to = Bishop;
            break;
        case 'r':
            ret.promoting_to = Rook;
            break;
        case 'q':
            ret.promoting_to = Queen;
            break;
        default:
            throw std::invalid_argument("Promotion piece must be one of 'nbrq'"
            );
        }
    ret.target_square = Coords::from_algebraic(move.substr(2, 2)).to_index();
    return ret;
}
