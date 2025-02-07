#include "piece.hpp"

#include "../board/board.hpp"
#include "../utils/coords.hpp"
#include "../utils/move.hpp"

std::vector<Move>
keep_only_blocking(const std::vector<Move> candidates, const Board& board) {
    if (candidates.size() == 0)
        return {};

    Colour my_colour = board.colour_at(candidates[0].source_square);
    std::vector<Move> ret;
    for (Move move : candidates) {
        Board board_after_move = board.make_move(move, false);
        board_after_move = board_after_move.skip_turn();

        if (!board_after_move.is_check())
            ret.push_back(move);
    }
    return ret;
}

std::vector<Move>
legal_moves(int8_t p, const Board& b, const Coords xy, bool looking_for_check) {
    std::vector<Move> ret;
    int8_t simple_piece = p & 0b00111;
    switch (simple_piece) {
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

    return ret;
}

std::optional<Move>
move_for_position(const Board& board, const Coords source, const Coords dest) {
    if (!dest.is_within_bounds()
        || board.colour_at(source) == board.colour_at(dest))
        return {};

    return Move{source.to_index(), dest.to_index()};
}

std::vector<Move>
look_direction(const Board& board, const Coords xy, int mult_dx, int mult_dy) {
    std::vector<Move> ret;
    for (int d = 1; d < 8; d++) {
        int dx = mult_dx * d;
        int dy = mult_dy * d;

        Coords target{xy.x + dx, xy.y + dy};
        std::optional<Move> move = move_for_position(board, xy, target);
        if (move.has_value()) {
            ret.push_back(move.value());
            if (board.squares[target.to_index()] != Piece::None)
                break;
        } else {
            break;
        }
    }
    return ret;
}
