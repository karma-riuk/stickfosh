#include "ai.hpp"

#include <thread>

Move ai::v0_random::_search(const Board& b) {
    std::vector<Move> moves = b.all_legal_moves();

    return moves[rand() % moves.size()];
}
