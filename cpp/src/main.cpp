#include "controller/ai_vs_ai.hpp"
#include "controller/controller.hpp"
#include "controller/human_vs_ai.hpp"
#include "controller/manual.hpp"
#include "view/gui.hpp"
#include "view/noop.hpp"
#include "view/view.hpp"

#include <chrono>

int main(int argc, char* argv[]) {
    std::string pos =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board b = Board::setup_fen_position(pos);

    ai::v0_random p1(std::chrono::milliseconds(500));
    ai::v0_random p2(std::chrono::milliseconds(500));

    GUI gui;
    AIvsAIController manual(b, gui, p1, p2);

    Controller& controller = manual;

    controller.start();

    // perft();
    // ai::v1_simple ai;
    //
    // Board b = Board::setup_fen_position(pos);
    // Move move = ai.search(b, true);
    // std::cout << move << std::endl;
    return 0;
}
