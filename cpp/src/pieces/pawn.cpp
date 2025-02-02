#include "../board.hpp"
#include "../coords.hpp"
#include "../move.hpp"
#include "piece.hpp"

std::vector<Move> pawn_moves(const Board& b, const Coords xy) {
    std::vector<Move> ret{};
    int8_t me = b.squares[xy.to_index()];
    int8_t my_colour = me & 0b11000;

    // -- Capture to the left
    if (xy.x > 0) {
        int dy = my_colour == Colour::White ? 1 : -1;
        Coords left{xy.x - 1, xy.y + dy};
        int8_t capturable_piece = b.squares[left.to_index()];
        if (capturable_piece != 0) {
            if (my_colour != (capturable_piece & 0b11000))
                ret.push_back(Move{xy.to_index(), left.to_index()});
        }
    }

    // -- Capture to the right
    if (xy.x < 7) {
        int dy = my_colour == Colour::White ? 1 : -1;
        Coords right{xy.x + 1, xy.y + dy};
        int8_t capturable_piece = b.squares[right.to_index()];
        if (capturable_piece != 0) {
            if (my_colour != (capturable_piece & 0b11000))
                ret.push_back(Move{xy.to_index(), right.to_index()});
        }
    }

    bool is_on_starting_rank =
        my_colour == Colour::White ? xy.y == 1 : xy.y == 6;
    int max_dy = is_on_starting_rank ? 3 : 2;
    for (int dy = 1; dy < max_dy; dy++) {
        int actual_dy = my_colour == Colour::White ? dy : -dy;
        Coords new_xy{xy.x, xy.y + actual_dy};
        if (b.squares[new_xy.to_index()] != Piece::None)
            break;
        if (new_xy.y == 7 || new_xy.y == 0)
            for (auto piece : {Queen, Knigt, Bishop, Rook})
                ret.push_back(Move{
                    xy.to_index(),
                    new_xy.to_index(),
                    .promoting_to = piece
                });
    }

    return ret;
}
