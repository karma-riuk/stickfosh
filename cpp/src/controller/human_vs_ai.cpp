#include "human_vs_ai.hpp"

#include <algorithm>

HumanVsAIController::HumanVsAIController(Board b, View& v, ai::AI& ai)
    : ManualController(b, v),
      ai(ai) {
    view.set_controller(this);
    reset_selection();
}

void HumanVsAIController::on_tile_selected(int x, int y) {
    Coords c{x, y};
    Piece piece = board.piece_at(c);

    if (selected_index == -1
        || (piece != Piece::None && piece != selected_piece
            && (piece & 0b11000) != (selected_piece & 0b11000))) {
        show_legal_moves(c);
    } else {
        auto boh = std::find(targets.begin(), targets.end(), c.to_index());
        if (boh != targets.end()) {
            make_move(Move{selected_index, c.to_index()});

            if (board.is_terminal())
                return;
            Move ai_move = ai.search(board);
            make_move(ai_move);
        } else
            reset_selection();
    }
}
