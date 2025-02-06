#include "ai_vs_ai.hpp"

#include "controller.hpp"

#include <thread>

AIvsAIController::AIvsAIController(Board b, View& v, ai::AI& p1, ai::AI& p2)
    : Controller(b, v),
      p1(p1),
      p2(p2) {}

void AIvsAIController::start() {
    std::thread view_thread([&]() { view.show(); });
    ai::AI* current_player;
    while (!board.is_terminal()) {
        current_player = board.white_to_play ? &p1 : &p2;
        Move move = current_player->search(board, board.white_to_play);
        make_move(move);
    }

    view_thread.join();
}

void AIvsAIController::make_move(Move move) {
    board = board.make_move(move);
    view.update_board(board, -1, {});

    Colour current_colour = board.white_to_play ? White : Black;
    if (board.is_checkmate_for(current_colour))
        view.notify_checkmate(current_colour);

    if (board.is_stalemate_for(current_colour))
        view.notify_stalemate(current_colour);
}

void AIvsAIController::on_tile_selected(int, int) {}
