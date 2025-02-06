#include "manual.hpp"

ManualController::ManualController(Board b, View& view): view(view) {
    board = b;
    selected_piece = Piece::None;
    legal_moves = {};

    view.update_board(b, selected_piece, legal_moves);
}

void ManualController::on_tile_selected(int x, int y) {}

void ManualController::reset_selection() {}

void ManualController::show_legal_moves(Coords) {}

void ManualController::make_move(Move) {}
