#include "ai.hpp"

Move ai::v0_random::search(const Board& b, bool) {
    std::vector<Move> moves = b.all_legal_moves();

    return moves[rand() % moves.size()];
}
