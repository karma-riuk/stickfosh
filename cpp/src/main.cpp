#include "controller/ai_vs_ai.hpp"
#include "controller/controller.hpp"
#include "controller/human_vs_ai.hpp"
#include "controller/manual.hpp"
#include "model/perft/perft.hpp"
#include "view/gui.hpp"
#include "view/noop.hpp"
#include "view/view.hpp"

#include <chrono>

int main(int argc, char* argv[]) {
    // std::string pos =
    //     "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string pos =
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 4 3";

    Board b = Board::setup_fen_position(pos);

    // ai::v0_random p1(true, std::chrono::milliseconds(500));
    ai::v1_simple p1(false, std::chrono::milliseconds(100000));
    // ai::v1_simple p2(false, std::chrono::milliseconds(100000));
    // ai::v0_random p2(false, std::chrono::milliseconds(10000));

    GUI gui;
    // AIvsAIController manual(b, gui, p1, p2);
    HumanVsAIController manual(b, gui, p1);

    Controller& controller = manual;

    controller.start();

    // perft(pos);
    return 0;
}
