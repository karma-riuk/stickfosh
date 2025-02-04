#include "board.hpp"
#include "stickfosh.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    std::string pos =
        "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 5";
    // Board b = Board::setup_fen_position(pos);
    perft();
    return 0;
}
