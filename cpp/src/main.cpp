#include "model/ais/ai.hpp"
#include "model/board/board.hpp"
#include "model/perft/perft.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    std::string pos =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    perft();
    // ai::v1_simple ai;
    //
    // Board b = Board::setup_fen_position(pos);
    // Move move = ai.search(b, true);
    // std::cout << move << std::endl;
    return 0;
}
