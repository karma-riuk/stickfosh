#include "controller/ai_vs_ai.hpp"
#include "controller/controller.hpp"
#include "controller/human_vs_ai.hpp"
#include "controller/manual.hpp"
#include "model/ais/ai.hpp"
#include "model/perft/perft.hpp"
#include "view/gui.hpp"
#include "view/noop.hpp"
#include "view/view.hpp"

#include <chrono>

int main(int argc, char* argv[]) {
    // std::string pos =
    //     "r2qkb1r/2p1pppp/p1n1b3/1p6/B2P4/2P1P3/P4PPP/R1BQK1NR w KQkq - 0 9 ";
    std::string pos = "3r4/3r4/3k4/8/3K4/8/8/8 w - - 0 1";

    // pos for ai timing<
    // std::string pos =
    //     "r3k2r/p1ppqpb1/Bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/R3K2R b KQkq - 0
    //     3";

    Board b = Board::setup_fen_position(pos);

    ai::v0_random p1(true, std::chrono::milliseconds(1000));
    // ai::v1_pure_minimax p2(false, std::chrono::milliseconds(20000));
    // ai::v2_alpha_beta p2(false, std::chrono::milliseconds(20000));
    // ai::v3_AB_ordering p2(false, std::chrono::milliseconds(20000));
    // ai::v4_search_captures p2(false, std::chrono::milliseconds(20000));
    // ai::v5_better_endgame p2(false, std::chrono::milliseconds(20000));
    ai::v6_iterative_deepening p2(false, std::chrono::milliseconds(2000));

    GUI gui;
    // NoOpView gui;
    // AIvsAIController manual(b, gui, p1, p2);
    HumanVsAIController manual(b, gui, p2);

    Controller& controller = manual;

    controller.start();

    // perft();
    return 0;
}
