#include "manual.hpp"

#include "../model/utils/utils.hpp"

#include <algorithm>

ManualController::ManualController(Board b, View& v): Controller(b, v) {
    reset_selection();
}

void ManualController::start() {
    reset_selection();
    view.show();
}

void ManualController::on_tile_selected(int x, int y) {
    Coords c{x, y};
    Piece piece = board.piece_at(c);

    if (selected_index == -1
        || (piece != Piece::None && piece != selected_piece
            && (piece & 0b11000) != (selected_piece & 0b11000))) {
        show_legal_moves(c);
    } else {
        auto boh = std::find(targets.begin(), targets.end(), c.to_index());
        if (boh != targets.end())
            make_move(Move{selected_index, c.to_index()});
        else
            reset_selection();
    }
}

void ManualController::reset_selection() {
    selected_index = -1;
    selected_piece = Piece::None;
    targets = {};
    view.update_board(board, selected_index, targets);
}

void ManualController::show_legal_moves(Coords xy) {
    Colour colour = board.colour_at(xy);
    if (colour) {
        Colour to_play = board.white_to_play ? White : Black;
        if (colour != to_play)
            return;
        selected_index = xy.to_index();
        selected_piece = board.piece_at(xy);
        targets = to_target_square(legal_moves(selected_piece, board, xy));
        view.update_board(board, xy.to_index(), targets);
    }
}

void ManualController::make_move(Move move) {
    // handle promotion before making the move
    Colour colour = board.white_to_play ? White : Black;
    Coords source = Coords::from_index(move.source_square);
    if (board.piece_at(move.source_square) == Piece::Pawn
        && board.colour_at(move.source_square) == White && source.y == 6) {
        Piece promotion_piece = (Piece) (colour | view.ask_about_promotion());
        move.promoting_to = promotion_piece;
    }

    std::cout << "Move made: " << move << std::endl;

    board = board.make_move(move);
    reset_selection();

    Colour current_colour = board.white_to_play ? White : Black;
    if (board.is_checkmate())
        view.notify_checkmate(current_colour);

    if (board.is_stalemate())
        view.notify_stalemate(current_colour);
}
