#include "board.hpp"
#include "perft.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    std::string pos =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // Board b = Board::setup_fen_position(pos);
    perft();
    return 0;
}
