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
#include <iostream>
#include <string>

void print_usage() {
    std::cout << "Usage: chess_ai [OPTIONS]\n";
    std::cout << "Options:\n";
    std::cout
        << "  --mode <human_vs_ai|ai_vs_ai|human_vs_human|perft>  Choose the "
           "game mode.\n";
    std::cout << "  --ai1 <version>  Choose the first AI version (for ai_vs_ai "
                 "mode).\n";
    std::cout << "  --ai2 <version>  Choose the second AI version (for "
                 "ai_vs_ai mode).\n";
    std::cout << "  --fen <FEN_STRING>  Set a custom FEN position.\n";
    std::cout << "  --help  Show this help message.\n";
}

int main(int argc, char* argv[]) {
    std::string mode = "human_vs_ai";
    std::string ai1_version = "v0_random";
    std::string ai2_version = "v6_iterative_deepening";
    std::string fen =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode" && i + 1 < argc) {
            mode = argv[++i];
        } else if (arg == "--ai1" && i + 1 < argc) {
            ai1_version = argv[++i];
        } else if (arg == "--ai2" && i + 1 < argc) {
            ai2_version = argv[++i];
        } else if (arg == "--fen" && i + 1 < argc) {
            fen = argv[++i];
        } else if (arg == "--help") {
            print_usage();
            return 0;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            print_usage();
            return 1;
        }
    }

    Board board = Board::setup_fen_position(fen);
    GUI gui;
    Controller* controller = nullptr;

    if (mode == "human_vs_ai") {
        ai::v6_iterative_deepening ai(false, std::chrono::milliseconds(2000));
        controller = new HumanVsAIController(board, gui, ai);
    } else if (mode == "ai_vs_ai") {
        ai::v0_random p1(true, std::chrono::milliseconds(1000));
        ai::v6_iterative_deepening p2(false, std::chrono::milliseconds(2000));
        controller = new AIvsAIController(board, gui, p1, p2);
    } else if (mode == "human_vs_human") {
        controller = new ManualController(board, gui);
    } else if (mode == "perft") {
        perft();
        return 0;
    } else {
        std::cerr << "Invalid mode selected!\n";
        print_usage();
        return 1;
    }

    controller->start();
    delete controller;
    return 0;
}
