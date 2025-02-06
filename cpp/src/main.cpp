#include "ais/ai.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    std::string pos =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    ai::v1_simple ai;

    std::string move = ai.search(pos, 4);
    std::cout << move << std::endl;
    return 0;
}
