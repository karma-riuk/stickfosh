#include "piece.hpp"

#include "../board.hpp"
#include "../coords.hpp"

std::vector<Move>
keep_only_blocking(const std::vector<Move> candidates, const Board& board) {
    if (candidates.size() == 0)
        return {};

    int8_t my_colour = board.colour_at(candidates[0].source_square);
    std::vector<Move> ret;
    for (Move move : candidates) {
        Board board_after_move = board.make_move(move);
        if (!board_after_move.is_check_for(my_colour))
            ret.push_back(move);
    }
    return ret;
}

std::vector<Move>
legal_moves(int8_t p, const Board& b, const Coords xy, bool looking_for_check) {
    std::vector<Move> ret;
    switch (p) {
    case Piece::Pawn:
        ret = pawn_moves(b, xy);
        break;
    case Piece::Bishop:
        ret = bishop_moves(b, xy);
        break;
    case Piece::Rook:
        ret = rook_moves(b, xy);
        break;
    case Piece::Knigt:
        ret = knight_moves(b, xy);
        break;
    case Piece::Queen:
        ret = queen_moves(b, xy);
        break;
    case Piece::King:
        ret = king_moves(b, xy);
        break;
    default:
        break;
    }

    if (!looking_for_check)
        return keep_only_blocking(ret, b);

    return {};
}

std::optional<Move>
move_for_position(const Board& board, const Coords source, const Coords dest) {
    if (dest.is_within_bounds())
        return {};

    int8_t piece = board.squares[dest.to_index()];
    if (piece == Piece::None)
        return Move{source.to_index(), dest.to_index()};

    int8_t source_colour = board.colour_at(source);
    int8_t dest_colour = board.colour_at(dest);
    if (source_colour != dest_colour)
        return Move{source.to_index(), dest.to_index(), true};
    return {};
}

std::vector<Move>
look_direction(const Board& board, const Coords xy, int mult_dx, int mult_dy) {
    std::vector<Move> ret;
    for (int d = 1; d < 8; d++) {
        int dx = mult_dx * d;
        int dy = mult_dy * d;

        std::optional<Move> move =
            move_for_position(board, xy, Coords{xy.x + dx, xy.y + dy});
        if (move.has_value()) {
            ret.push_back(move.value());
            if (move.value().is_capturing)
                break;
        } else {
            break;
        }
    }
    return ret;
}
