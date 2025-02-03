#include "../board.hpp"
#include "../coords.hpp"
#include "../move.hpp"
#include "piece.hpp"

std::vector<Move> pawn_moves(const Board& b, const Coords xy) {
    std::vector<Move> ret{};
    int8_t my_colour = b.colour_at(xy);

    // -- Capture to the left
    if (xy.x > 0) {
        int dy = my_colour == Colour::White ? 1 : -1;
        Coords left{xy.x - 1, xy.y + dy};
        int8_t capturable_piece = b.squares[left.to_index()];
        if (capturable_piece != 0) {
            if (my_colour != b.colour_at(left))
                if ((my_colour == White && left.y == 7)
                    || (my_colour == Black && left.y == 0))

                    for (auto piece : {Rook, Knigt, Bishop, Queen})
                        ret.push_back(Move{
                            xy.to_index(),
                            left.to_index(),
                            .is_capturing = true,
                            .promoting_to = (int8_t) (my_colour | piece)
                        });
                else
                    ret.push_back(Move{xy.to_index(), left.to_index()});
        }
    }

    // -- Capture to the right
    if (xy.x < 7) {
        int dy = my_colour == Colour::White ? 1 : -1;
        Coords right{xy.x + 1, xy.y + dy};
        int8_t capturable_piece = b.squares[right.to_index()];
        if (capturable_piece != 0) {
            if (my_colour != b.colour_at(right))
                if ((my_colour == White && right.y == 7)
                    || (my_colour == Black && right.y == 0))

                    for (auto piece : {Rook, Knigt, Bishop, Queen})
                        ret.push_back(Move{
                            xy.to_index(),
                            right.to_index(),
                            .is_capturing = true,
                            .promoting_to = (int8_t) (my_colour | piece)
                        });
                else
                    ret.push_back(Move{xy.to_index(), right.to_index()});
        }
    }

    // -- Capture en passant
    if (b.en_passant_target != -1) {
        Coords c = Coords::from_index(b.en_passant_target);
        int dy = my_colour == Colour::White ? 1 : -1;
        if (c.y == xy.y + dy && (c.x == xy.x - 1 || c.x == xy.x + 1)) {
            ret.push_back(Move{
                xy.to_index(),
                c.to_index(),
                .is_capturing = true,
                .en_passant = true
            });
        }
    }

    // -- Normal move + promotion
    bool is_on_starting_rank =
        my_colour == Colour::White ? xy.y == 1 : xy.y == 6;
    int max_dy = is_on_starting_rank ? 3 : 2;
    for (int dy = 1; dy < max_dy; dy++) {
        int actual_dy = my_colour == Colour::White ? dy : -dy;
        Coords new_xy{xy.x, xy.y + actual_dy};
        if (b.squares[new_xy.to_index()] != Piece::None)
            break;
        if (new_xy.y == 7 || new_xy.y == 0)
            for (auto piece : {Rook, Knigt, Bishop, Queen})
                ret.push_back(Move{
                    xy.to_index(),
                    new_xy.to_index(),
                    .promoting_to = (int8_t) (my_colour | piece)
                });
        else
            ret.push_back(Move{
                xy.to_index(),
                new_xy.to_index(),
            });
    }

    return ret;
}
