#include "ai.hpp"

#include <thread>

Move ai::v0_random::_search(const Board& b) {
    std::vector<Move> moves = b.all_legal_moves();


    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate work

    return moves[rand() % moves.size()];
}
