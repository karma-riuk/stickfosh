#include "../board.hpp"
#include "../coords.hpp"
#include "piece.hpp"

static bool is_clear_king_side(const Board& b, const Coords xy) {
    for (int dx = 1; dx < 3; dx++) {
        Coords c{xy.x + dx, xy.y};
        if (b.squares[c.to_index()] != Piece::None)
            return false;

        std::optional<Move> move = move_for_position(b, xy, c);
        Board board_after_move = b.make_move(move.value());
        if (board_after_move.is_check_for(b.colour_at(xy)))
            return false;
    }
    return true;
}

static bool is_clear_queen_side(const Board& b, const Coords xy) {
    for (int dx = 1; dx < 4; dx++) {
        Coords c{xy.x - dx, xy.y};
        if (b.squares[c.to_index()] != Piece::None)
            return false;

        std::optional<Move> move = move_for_position(b, xy, c);
        Board board_after_move = b.make_move(move.value());
        if (dx < 3 && board_after_move.is_check_for(b.colour_at(xy)))
            return false;
    }
    return true;
}

std::vector<Move> king_moves(const Board& b, const Coords xy) {
    std::vector<Move> ret;

    // -- Regular moves
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) // skip staying in the same position
                continue;
            Coords c{xy.x + dx, xy.y + dy};
            std::optional<Move> move = move_for_position(b, xy, c);
            if (move.has_value())
                ret.push_back(move.value());
        }
    }

    if (b.is_check_for(b.colour_at(xy)))
        return ret;

    // -- Castles
    CastleSide castling_rights = b.colour_at(xy) == Colour::White
                                   ? b.w_castle_rights
                                   : b.b_castle_rights;

    if (castling_rights == CastleSide::Neither)
        return ret;

    if (castling_rights == CastleSide::King && is_clear_king_side(b, xy)) {
        ret.push_back(Move{
            xy.to_index(),
            Coords{6, xy.y}.to_index(),
            .castle_side = CastleSide::King
        });
    }

    if (castling_rights == CastleSide::Queen && is_clear_queen_side(b, xy)) {
        ret.push_back(Move{
            xy.to_index(),
            Coords{2, xy.y}.to_index(),
            .castle_side = CastleSide::Queen
        });
    }

    return ret;
}
